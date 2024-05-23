#include "alarm.h"
#include <limits.h>
#include <stdlib.h>

// Returns 0 if a <= b < c, 1 otherwise
static int within_range(uint32_t a, uint32_t b, uint32_t c) {
  return (b - a) < (b - c);
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

int libtock_alarm_in_ms(uint32_t ms, libtock_alarm_callback cb, void* opaque, libtock_alarm_t* alarm) {
  uint32_t frequency;
  libtock_alarm_command_get_frequency(&frequency);
  uint32_t interval = (ms / 1000) * frequency + (ms % 1000) * (frequency / 1000);
  uint32_t now;
  libtock_alarm_command_read(&now);
  return libtock_alarm_at(now, interval, cb, opaque, alarm);
}

static void alarm_repeating_cb(uint32_t now, __attribute__ ((unused)) uint32_t scheduled, void* opaque) {
  libtock_alarm_repeating_t* repeating = (libtock_alarm_repeating_t*) opaque;
  uint32_t interval = repeating->interval;
  uint32_t cur_exp  = repeating->alarm.reference + interval;
  libtock_alarm_at_internal(cur_exp, interval, (libtock_alarm_callback)alarm_repeating_cb,
                            (void*)repeating, &repeating->alarm);
  repeating->cb(now, cur_exp, repeating->ud);
}


void libtock_alarm_repeating_every(uint32_t ms, libtock_alarm_callback cb, void* opaque,
                                   libtock_alarm_repeating_t* repeating) {
  uint32_t frequency;
  libtock_alarm_command_get_frequency(&frequency);
  uint32_t interval = (ms / 1000) * frequency + (ms % 1000) * (frequency / 1000);

  repeating->interval = interval;
  repeating->cb       = cb;
  repeating->ud       = opaque;

  uint32_t now;
  libtock_alarm_command_read(&now);
  libtock_alarm_at_internal(now, interval, alarm_repeating_cb,
                            (void*)repeating, &repeating->alarm);
}

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
