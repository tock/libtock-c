#include "alarm.h"
#include <assert.h>
#include <limits.h>
#include <stdlib.h>

#define MAX_TICKS UINT32_MAX

// Returns 0 if a <= b < c, 1 otherwise
static int within_range(uint32_t a, uint32_t b, uint32_t c) {
  return (b - a) < (b - c);
}

/** \brief Convert milliseconds to clock ticks
 *
 * WARNING: This function will assert if the output
 * number of ticks overflows `UINT32_MAX`.
 *
 * This conversion is accurate to within 1 millisecond of a true
 * fractional conversion.
 *
 * \param ms the milliseconds to convert to ticks
 * \return ticks a number of clock ticks that
 * correspond to the given number of milliseconds
 */
static uint32_t ms_to_ticks(uint32_t ms) {
  // This conversion has a max error of 1ms.
  // View the justification here https://github.com/tock/libtock-c/pull/434
  uint32_t frequency;
  libtock_alarm_command_get_frequency(&frequency);

  uint32_t seconds                 = ms / 1000;
  uint32_t leftover_millis         = ms % 1000;
  uint32_t milliseconds_per_second = 1000;

  // ticks needs to be 64 bits because the kernel may scale frequency several magnitudes
  uint64_t ticks = (uint64_t) seconds * frequency;
  ticks += ((uint64_t) leftover_millis * frequency) / milliseconds_per_second;

  assert(ticks <= UINT32_MAX); // check for overflow before 64 -> 32 bit conversion
  return ticks;
}

static uint32_t ticks_to_ms(uint32_t ticks) {
  // `ticks_to_ms`'s conversion will be accurate to within the range
  // 0 to 1 milliseconds less than the exact conversion
  // (true millisecond conversion - [0,1) milliseconds).

  uint32_t frequency;
  libtock_alarm_command_get_frequency(&frequency);

  uint32_t seconds = (ticks / frequency);
  uint32_t milliseconds_per_second = 1000;

  // Calculate the conversion of full seconds to ticks.
  uint32_t milliseconds = seconds * milliseconds_per_second;

  // To get conversion accuracy within 1 millisecond, the conversion
  // must also convert partial seconds.
  uint32_t leftover_ticks = ticks % frequency;

  // This calculation is mathematically equivalent to doing:
  //
  //   `leftover_ticks` / (`frequency` / `milliseconds_per_second`)
  //
  // This is taking the remaining unconverted ticks (leftover_ticks)
  // and dividing by the number of ticks per millisecond
  // (`frequency` (ticks per second) / `1000` milliseconds per second)
  // The division is done this way because of the same argument in
  // `ms_to_ticks`.
  milliseconds += (leftover_ticks * milliseconds_per_second) / frequency;

  return milliseconds;
}

static libtock_alarm_t* root = NULL;

static void root_insert(libtock_alarm_t* alarm) {
  if (root == NULL) {
    root       = alarm;
    root->next = NULL;
    root->prev = NULL;
    return;
  }

  libtock_alarm_t** cur = &root;
  libtock_alarm_t* prev = NULL;
  while (*cur != NULL) {
    uint32_t cur_expiration = (*cur)->reference + (*cur)->dt;
    uint32_t new_expiration = alarm->reference + alarm->dt;
    if (!within_range(alarm->reference, cur_expiration, new_expiration)) {
      // insert before
      libtock_alarm_t* tmp = *cur;
      *cur        = alarm;
      alarm->next = tmp;
      alarm->prev = prev;
      tmp->prev   = alarm;
      return;
    }
    prev = *cur;
    cur  = &prev->next;
  }
  // didn't return, so prev points to the last in the list
  prev->next  = alarm;
  alarm->prev = prev;
  alarm->next = NULL;

}

static libtock_alarm_t* root_pop(void) {
  if (root == NULL) {
    return NULL;
  } else {
    libtock_alarm_t* res = root;
    root = root->next;
    if (root != NULL) {
      root->prev = NULL;
    }
    res->next = NULL;
    return res;
  }
}

static libtock_alarm_t* root_peek(void) {
  return root;
}

static void alarm_upcall(__attribute__ ((unused)) int   kernel_now,
                         __attribute__ ((unused)) int   scheduled,
                         __attribute__ ((unused)) int   unused2,
                         __attribute__ ((unused)) void* opaque) {
  for (libtock_alarm_t* alarm = root_peek(); alarm != NULL; alarm = root_peek()) {
    uint32_t now;
    libtock_alarm_command_read(&now);
    // has the alarm not expired yet? (distance from `now` has to be larger or
    // equal to distance from current clock value.
    if (alarm->dt > now - alarm->reference) {
      libtock_alarm_command_set_absolute(alarm->reference, alarm->dt);
      break;
    } else {
      root_pop();

      if (alarm->callback) {
        uint32_t expiration = alarm->reference + alarm->dt;
        alarm->callback(now, expiration, alarm->ud);
      }
    }
  }
}

static int libtock_alarm_at_internal(uint32_t reference, uint32_t dt, libtock_alarm_callback cb, void* ud,
                                     libtock_alarm_t* alarm) {
  alarm->reference = reference;
  alarm->dt        = dt;
  alarm->callback  = cb;
  alarm->ud        = ud;
  alarm->prev      = NULL;
  alarm->next      = NULL;

  root_insert(alarm);
  int i = 0;
  for (libtock_alarm_t* cur = root_peek(); cur != NULL; cur = cur->next) {
    i++;
  }

  if (root_peek() == alarm) {
    libtock_alarm_set_upcall((subscribe_upcall*)alarm_upcall, NULL);

    return libtock_alarm_command_set_absolute(alarm->reference, alarm->dt);
  }
  return RETURNCODE_SUCCESS;
}

int libtock_alarm_at(uint32_t reference, uint32_t dt, libtock_alarm_callback cb, void* opaque, libtock_alarm_t* alarm) {
  return libtock_alarm_at_internal(reference, dt, cb, opaque, alarm);
}

void libtock_alarm_cancel(libtock_alarm_t* alarm) {
  if (alarm->prev != NULL) {
    alarm->prev->next = alarm->next;
  }
  if (alarm->next != NULL) {
    alarm->next->prev = alarm->prev;
  }

  if (root == alarm) {
    root = alarm->next;
    if (root != NULL) {
      libtock_alarm_command_set_absolute(root->reference, root->dt);
    }
  }

  alarm->prev = NULL;
  alarm->next = NULL;
}

// The intermediate callback that handles overflows of alarm. This is used by
// `timer_in` to handle cases where `alarm_at` would normally be unable to
// support the whole timer length. `alarm_at` can only keep track of up to 2^32
// ticks.
static void overflow_callback(__attribute__ ((unused)) uint32_t now,
                              uint32_t                          last_timer_fire_time,
                              void*                             overflow_ud) {
  libtock_alarm_repeating_t* tock_timer = (libtock_alarm_repeating_t*)overflow_ud;

  if (tock_timer->overflows_left == 0) {
    // no overflows left, schedule last alarm with original callback
    libtock_alarm_at(last_timer_fire_time,
                     tock_timer->remaining_ticks,
                     tock_timer->callback,
                     tock_timer->user_data,
                     &(tock_timer->alarm));
  } else {
    // schedule next intermediate alarm that will overflow
    tock_timer->overflows_left--;

    libtock_alarm_at(last_timer_fire_time,
                     MAX_TICKS,
                     (libtock_alarm_callback) overflow_callback,
                     (void*) tock_timer,
                     &(tock_timer->alarm));
  }
}

int libtock_alarm_in_ms(uint32_t ms, libtock_alarm_callback cb, void* opaque, libtock_alarm_repeating_t* alarm) {
  uint32_t now;
  int ret = libtock_alarm_command_read(&now);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // If `dt_ms` is longer than the time that an alarm can count up to, then `timer_in` will
  // schedule multiple alarms to reach the full length. We calculate the number of full overflows
  // and the remainder ticks to reach the target length of time. The overflows use the
  // `overflow_callback` for each intermediate overflow.
  const uint32_t max_ticks_in_ms = ticks_to_ms(MAX_TICKS);
  if (ms > max_ticks_in_ms) {
    // overflows_left is the number of intermediate alarms that need to be scheduled to reach the target
    // dt_ms. After the alarm in this block is scheduled, we have this many overflows left (hence the reason
    // for subtracting by one). Subtracting by one is safe and will not underflow because we have already
    // checked that dt_ms > max_ticks_in_ms.
    alarm->overflows_left  = (ms / max_ticks_in_ms) - 1;
    alarm->remaining_ticks = ms_to_ticks(ms % max_ticks_in_ms);
    alarm->user_data       = opaque;
    alarm->callback        = cb;

    return libtock_alarm_at(now, MAX_TICKS, (libtock_alarm_callback)overflow_callback, (void*)(alarm),
                            &(alarm->alarm));
  } else {
    // No overflows needed
    return libtock_alarm_at(now, ms_to_ticks(ms), cb, opaque, &(alarm->alarm));
  }
}

static void alarm_repeating_cb(uint32_t now, __attribute__ ((unused)) uint32_t scheduled, void* opaque) {
  libtock_alarm_repeating_t* repeating = (libtock_alarm_repeating_t*) opaque;
  uint32_t interval_ms = repeating->interval_ms;
  // It's possible for the call to ms_to_ticks to overflow if interval_ms is greater
  // than 2^32 ticks, but the wraparound gives use the expiration time we want.
  uint32_t cur_exp = repeating->alarm.reference + ms_to_ticks(interval_ms);

  libtock_alarm_in_ms(interval_ms, (libtock_alarm_callback)alarm_repeating_cb, (void*)repeating, repeating);
  repeating->callback(now, cur_exp, repeating->user_data);
}


void libtock_alarm_repeating_every_ms(uint32_t ms, libtock_alarm_callback cb, void* opaque,
                                      libtock_alarm_repeating_t* repeating) {
  repeating->interval_ms = ms;
  repeating->callback    = cb;
  repeating->user_data   = opaque;

  libtock_alarm_in_ms(ms, (libtock_alarm_callback)alarm_repeating_cb, (void*)repeating, repeating);
}

// TODO: should there also be a cancel called `libtock_alarm_in_ms_cancel`
void libtock_alarm_repeating_cancel(libtock_alarm_repeating_t* repeating) {
  libtock_alarm_cancel(&repeating->alarm);
}

int libtock_alarm_gettimeasticks(struct timeval* tv, __attribute__ ((unused)) void* tzvp) {
  uint32_t frequency, now, seconds, remainder;
  const uint32_t microsecond_scaler = 1000000;

  libtock_alarm_command_get_frequency(&frequency);
  libtock_alarm_command_read(&now);

  assert(frequency > 0);

  // Obtain seconds and remainder due to integer divison
  seconds   = now / frequency;
  remainder = now % frequency;

  // (ticks) * (1e6 us / s) * (s / ticks) = us
  // Because remainder is by definition less than
  // frequency, we must be sure to first scale remainder
  // or else floor(remainder / frequency) = 0. To prevent
  // an overflow, we cast remainder to be a `uint64_t`. By
  // integer promotion rules, microsecond_scaler and frequency
  // will subsequently be cast to `uint64_t`.
  uint64_t us = ((uint64_t) remainder * microsecond_scaler) / frequency;

  tv->tv_sec = seconds;

  // The maximum micro second value is 999999us,
  // as any value greater than this is a second.
  // Subsequently, we can safely cast the uint64_t
  // to a uint32_t.
  tv->tv_usec = (uint32_t) us;

  return 0;
}
