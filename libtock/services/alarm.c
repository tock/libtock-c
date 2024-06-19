#include "alarm.h"
#include <assert.h>
#include <stdlib.h>

#define MAX_TICKS UINT32_MAX

/** \brief Checks if `now` is between `reference` and `dt`, meaning
 *  the alarm has not yet expired.
 *
 * Invariants:
 * 1. `now` hasn't wrapped `reference`, i.e., in an infinite space,
 *    `now` - `reference` <= MAX_TICKS
 * 2. `dt` is at most MAX_TICKS, such that `reference + dt` at most
 *     wraps back to `reference`.
 * 3. `now` happens after `reference`, so if `now` is not "within"
 *    `reference` and `dt`, it is after, not before.
 * 4. `now` may be larger or smaller than `reference` since ticks
 *    wrap.
 *
 * Corrolaries:
 * - if `now` is larger than `reference`, `now - reference` is
 *   directly comparable to `dt` (1 & 2)
 * - if `now` is less than reference, `now - reference` wraps, and is
 * - comparable to `dt` because `now` is guaranteed to happened after
 *   `reference` (3)
 *
 * The result is that the check is fairly simple, but its simplicitly
 * relies specifically on `now` never happening before `reference`,
 * and thus tied to the specific logic in `alarm_upcall` (see inline
 * comment on delaying executing callbacks until after checking all
 * outstanding alarms).
 */
static bool is_within(uint32_t now, uint32_t reference, uint32_t dt) {
  return now - reference < dt;
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


// Declare non-public export of helper for libtock-sync.
uint32_t _ms_to_ticks(uint32_t ms);
/** \brief Private export of ms->ticks helper to libtock-sync
 *
 * This is a non-stable, non-public interface for a helper
 * function which is also useful to libtock-sync.
 */
uint32_t _ms_to_ticks(uint32_t ms) {
  return ms_to_ticks(ms);
}

uint32_t libtock_alarm_ticks_to_ms(uint32_t ticks) {
  // `ticks_to_ms`'s conversion will be accurate to within the range
  // 0 to 1 milliseconds less than the exact conversion
  // (true millisecond conversion - [0,1) milliseconds).

  uint32_t frequency;
  libtock_alarm_command_get_frequency(&frequency);

  uint64_t seconds = (ticks / frequency);
  uint64_t milliseconds_per_second = 1000;

  // Calculate the conversion of full seconds to ticks.
  uint64_t milliseconds = seconds * milliseconds_per_second;

  // To get conversion accuracy within 1 millisecond, the conversion
  // must also convert partial seconds.
  uint64_t leftover_ticks = ticks % frequency;

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

static libtock_alarm_ticks_t* root = NULL;

static void root_insert(libtock_alarm_ticks_t* alarm) {
  // We want to insert the new alarm just before an existing alarm
  // that should expire next after it, as `alarm_upcall` breaks as
  // soon as it finds an alarm that should not fire yet. To do so, we
  // need to account for an empty list, a non-empty list where the new
  // alarm should be last, as well as computing relative expirations
  // when alarm expirations may overflow the clock 0 or 1 times.

  if (root == NULL) {
    root       = alarm;
    root->next = NULL;
    root->prev = NULL;
    return;
  }

  // Compute the tick value at which the new alarm will expire.
  uint32_t new_expiration = alarm->reference + alarm->dt;
  // Determine whether the clock overflows before the new alarm
  // expires. Because ticks are 32-bit, a clock can overflow at most
  // once before a 32-bit alarm fires.
  bool new_overflows = alarm->reference > (UINT32_MAX - alarm->dt);

  libtock_alarm_ticks_t** cur = &root;
  libtock_alarm_ticks_t* prev = NULL;
  while (*cur != NULL) {
    // Compute the tick value at which this alarm will expire.
    uint32_t cur_expiration = (*cur)->reference + (*cur)->dt;
    // Determine whether the clock overflows before this alarm
    // expires.
    bool cur_overflows = (*cur)->reference > (UINT32_MAX - (*cur)->dt);

    // This alarm (`cur`) happens after the new alarm (`alarm`) if:
    //
    // - both overflow or neither overflow and cur expiration is
    //   larger than the new expiration
    // - only cur overflows, cur's reference is before new reference
    //   (both started this epoch, cur must expire next epoch)
    // - only cur overflows, cur's reference is after new reference
    //   (cur started in previous epoch, cur also must expire this
    //   epoch) and cur's expiration is larger than the new
    //   expiration.
    //
    // If the new alarm overflows and this alarm doesn't, this alarm
    // happens _before_ the new alarm.
    if (((cur_overflows == new_overflows) && (cur_expiration > new_expiration)) ||
        (cur_overflows && ((*cur)->reference < alarm->reference || cur_expiration > new_expiration))) {
      // insert before
      libtock_alarm_ticks_t* tmp = *cur;
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

static libtock_alarm_ticks_t* root_pop(void) {
  if (root == NULL) {
    return NULL;
  } else {
    libtock_alarm_ticks_t* res = root;
    root = root->next;
    if (root != NULL) {
      root->prev = NULL;
    }
    res->next = NULL;
    return res;
  }
}

static libtock_alarm_ticks_t* root_peek(void) {
  return root;
}

/** \brief Upcall for internal virtual alarms
 *
 * This upcall checks the ordered list of outstanding alarms for
 * expired alarms, removes them from the list, and invokes their
 * callbacks.
 *
 * Invariants:
 * 1. The list of outstanding alarms is ordered by absolute expiration time.
 * 2. The alarm event that invoked this upcall is for the current `head` of the list.
 * 3. No alarms are added (or re-added) to the list between while
 *    iterating through alarms
 * 4. For each alarm, `alarm->dt < MAX_TICKS + 1`
 *
 * Corrollaries:
 * - If the head of the list hasn't expired, no alarms in the tail of
 *   the list have expired (1)
 * - `scheduled` happens after `head->reference` (2)
 * - `scheduled` happens before `head->reference + MAX_TICKS + 1` (4)
 * - For each alarm in the list, `now` happens after
 *   `alarm->reference` (2, 3)
 * - For each alarm, `scheduled` cannot have cannot have wrapped
 *   `alarm->reference` (1, 4)
 * - For each alarm, if `scheduled` and `now` are not on the same side of
 *   `alarm->reference`, the alarm must have expired (1, 4)
 *
 * Critically, this upcall cannot allow any alarms to be added to the
 * ordered list while iterating by invoking upcalls, as that could
 * violate invariant (3) and result in `now` happening before some
 * `alarm->reference`.
 *
 * Some alarms that expire between `now` and the end of the upcall may
 * be "missed", which may mean they are delivered later. They should
 * still show up first in the list at the end, so will fire next.
 */
static void alarm_upcall(__attribute__ ((unused)) int   kernel_now,
                         int                            scheduled0,
                         __attribute__ ((unused)) int   unused2,
                         __attribute__ ((unused)) void* opaque) {
  // `tocall` is a temporary list to keep track of expired alarms to call later.
  libtock_alarm_ticks_t* tocall      = NULL;
  libtock_alarm_ticks_t* tocall_last = NULL;

  // We want `scheduled` as unsigned so wrapping math works out correctly.
  uint32_t scheduled = scheduled0;

  // Take the current tick value. We could use `kernel_now`, but would
  // potentially unnecessarily delay some alarms.
  uint32_t now;
  libtock_alarm_command_read(&now);

  // We know this upcall is associated with the head, so it's easier
  // to deal with.
  libtock_alarm_ticks_t* head = root_pop();

  // Formally, we should be able to just add head to
  // `tocall`, but let's just be defensive just in case there is an
  // errant alarm.
  assert(head != NULL && !is_within(scheduled, head->reference, head->dt));
  head->next  = NULL;
  head->prev  = NULL;
  tocall      = head;
  tocall_last = head;

  // Now iterate through the remaining alarms in case any of them have
  // also expired.
  for (libtock_alarm_ticks_t* alarm = root_peek(); alarm != NULL; alarm = root_peek()) {
    // has the alarm not expired yet?
    // Three cases:
    //   1. scheduled - reference is larger than now - reference:
    //      ticks have wrapped reference, alarm must have expired.
    //   2. scheduled is no longer within reference + dt (alarm
    //      expired)
    //   3. now is no longer within reference + dt (alarm expired)
    //
    // Simpler to check the non-expiring case first.
    if ((now - alarm->reference >= scheduled - alarm->reference) &&
        is_within(scheduled, alarm->reference, alarm->dt) &&
        is_within(now, alarm->reference, alarm->dt)) {
      // Nope, has not expired, nothing "after" this alarm has
      // expired either, since we add alarms in expiration order.
      break;
    } else {
      // Expired, add to `tocall` list.
      root_pop();
      alarm->next = NULL;
      alarm->prev = tocall_last;
      // If this expired, head must have also expired, so `tocall` and
      // `tocall_last` are non-null, so just add to the end.
      tocall_last->next = alarm;
    }
  }

  for (libtock_alarm_ticks_t* alarm = tocall; alarm != NULL; alarm = tocall) {
    alarm->prev = NULL;
    tocall      = alarm->next;
    alarm->next = NULL;
    if (alarm->callback) {
      uint32_t expiration = alarm->reference + alarm->dt;
      alarm->callback(now, expiration, alarm->ud);
    }
  }

  head = root_peek();
  if (head != NULL) {
    // TODO(alevy): At this point, is it possible we've wrapped so far
    // past `reference` that we might end up delaying a technically
    // expired alarm by another timer wrap? I think technically yes,
    // though techincally the interface only guarantees alarms will
    // delay *at least* `dt`, so more is fine, and we could be delayed
    // arbitrarily long by the kernel anyway.
    libtock_alarm_command_set_absolute(head->reference, head->dt);
  }
}

static int libtock_alarm_at_internal(uint32_t reference, uint32_t dt, libtock_alarm_callback cb, void* ud,
                                     libtock_alarm_ticks_t* alarm) {
  alarm->reference = reference;
  alarm->dt        = dt;
  alarm->callback  = cb;
  alarm->ud        = ud;
  alarm->prev      = NULL;
  alarm->next      = NULL;

  root_insert(alarm);

  if (root_peek() == alarm) {
    libtock_alarm_set_upcall((subscribe_upcall*)alarm_upcall, NULL);

    return libtock_alarm_command_set_absolute(alarm->reference, alarm->dt);
  }
  return RETURNCODE_SUCCESS;
}

int libtock_alarm_at(uint32_t reference, uint32_t dt, libtock_alarm_callback cb, void* opaque,
                     libtock_alarm_ticks_t* alarm) {
  return libtock_alarm_at_internal(reference, dt, cb, opaque, alarm);
}

void libtock_alarm_cancel(libtock_alarm_ticks_t* alarm) {
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
  libtock_alarm_t* tock_timer = (libtock_alarm_t*)overflow_ud;

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

    const uint32_t max_ticks_in_ms = libtock_alarm_ticks_to_ms(MAX_TICKS);
    const uint32_t max_ms_in_ticks = ms_to_ticks(max_ticks_in_ms);
    libtock_alarm_at(last_timer_fire_time,
                     max_ms_in_ticks,
                     (libtock_alarm_callback) overflow_callback,
                     (void*) tock_timer,
                     &(tock_timer->alarm));
  }
}

int libtock_alarm_in_ms(uint32_t ms, libtock_alarm_callback cb, void* opaque, libtock_alarm_t* alarm) {
  uint32_t now;
  int ret = libtock_alarm_command_read(&now);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // If `dt_ms` is longer than the time that an alarm can count up to, then `timer_in` will
  // schedule multiple alarms to reach the full length. We calculate the number of full overflows
  // and the remainder ticks to reach the target length of time. The overflows use the
  // `overflow_callback` for each intermediate overflow.
  const uint32_t max_ticks_in_ms = libtock_alarm_ticks_to_ms(MAX_TICKS);
  const uint32_t max_ms_in_ticks = ms_to_ticks(max_ticks_in_ms);
  if (ms > max_ticks_in_ms) {
    // overflows_left is the number of intermediate alarms that need to be scheduled to reach the target
    // dt_ms. After the alarm in this block is scheduled, we have this many overflows left (hence the reason
    // for subtracting by one). Subtracting by one is safe and will not underflow because we have already
    // checked that dt_ms > max_ticks_in_ms.
    alarm->overflows_left  = (ms / max_ticks_in_ms) - 1;
    alarm->remaining_ticks = ms_to_ticks(ms % max_ticks_in_ms);
    alarm->user_data       = opaque;
    alarm->callback        = cb;

    return libtock_alarm_at(now, max_ms_in_ticks, (libtock_alarm_callback)overflow_callback, (void*)(alarm),
                            &(alarm->alarm));
  } else {
    // No overflows needed
    return libtock_alarm_at(now, ms_to_ticks(ms), cb, opaque, &(alarm->alarm));
  }
}

static void alarm_repeating_cb(uint32_t now, __attribute__ ((unused)) uint32_t scheduled, void* opaque) {
  libtock_alarm_t* repeating = (libtock_alarm_t*) opaque;
  uint32_t interval_ms       = repeating->interval_ms;
  // It's possible for the call to ms_to_ticks to overflow if interval_ms is greater
  // than 2^32 ticks, but the wraparound gives use the expiration time we want.
  uint32_t cur_exp = repeating->alarm.reference + ms_to_ticks(interval_ms);

  libtock_alarm_in_ms(interval_ms, (libtock_alarm_callback)alarm_repeating_cb, (void*)repeating, repeating);
  repeating->callback(now, cur_exp, repeating->user_data);
}


void libtock_alarm_repeating_every_ms(uint32_t ms, libtock_alarm_callback cb, void* opaque,
                                      libtock_alarm_t* repeating) {
  repeating->interval_ms = ms;
  repeating->callback    = cb;
  repeating->user_data   = opaque;

  libtock_alarm_in_ms(ms, (libtock_alarm_callback)alarm_repeating_cb, (void*)repeating, repeating);
}

void libtock_alarm_ms_cancel(libtock_alarm_t* alarm) {
  libtock_alarm_cancel(&alarm->alarm);
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
