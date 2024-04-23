#include "alarm_timer.h"
#include <limits.h>
#include <stdlib.h>

// Returns 0 if a <= b < c, 1 otherwise
static int within_range(uint32_t a, uint32_t b, uint32_t c) {
  return (b - a) < (b - c);
}

static alarm_t* root = NULL;

static void root_insert(alarm_t* alarm) {
  if (root == NULL) {
    root       = alarm;
    root->next = NULL;
    root->prev = NULL;
    return;
  }

  alarm_t **cur = &root;
  alarm_t *prev = NULL;
  while (*cur != NULL) {
    uint32_t cur_expiration = (*cur)->reference + (*cur)->dt;
    uint32_t new_expiration = alarm->reference + alarm->dt;
    if (!within_range(alarm->reference, cur_expiration, new_expiration)) {
      // insert before
      alarm_t *tmp = *cur;
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

static alarm_t* root_pop(void) {
  if (root == NULL) {
    return NULL;
  } else {
    alarm_t *res = root;
    root = root->next;
    if (root != NULL) {
      root->prev = NULL;
    }
    res->next = NULL;
    return res;
  }
}

static alarm_t* root_peek(void) {
  return root;
}

static void alarm_timer_upcall( __attribute__ ((unused)) int   unused0,
                                __attribute__ ((unused)) int   unused1,
                                __attribute__ ((unused)) int   unused2,
                                __attribute__ ((unused)) void* ud) {
  for (alarm_t* alarm = root_peek(); alarm != NULL; alarm = root_peek()) {
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
        tock_enqueue(alarm->callback, now, expiration, 0, alarm->ud);
      }
    }
  }
}

int libtock_alarm_at(uint32_t reference, uint32_t dt, subscribe_upcall cb, void* ud, alarm_t* alarm) {
  alarm->reference = reference;
  alarm->dt        = dt;
  alarm->callback  = cb;
  alarm->ud        = ud;
  alarm->prev      = NULL;
  alarm->next      = NULL;

  root_insert(alarm);
  int i = 0;
  for (alarm_t* cur = root_peek(); cur != NULL; cur = cur->next) {
    i++;
  }

  if (root_peek() == alarm) {
    libtock_alarm_set_upcall((subscribe_upcall*)alarm_timer_upcall, NULL);

    return libtock_alarm_command_set(alarm->reference, alarm->dt);
  }
  return RETURNCODE_SUCCESS;
}

void libtock_alarm_cancel(alarm_t* alarm) {
  if (alarm->prev != NULL) {
    alarm->prev->next = alarm->next;
  }
  if (alarm->next != NULL) {
    alarm->next->prev = alarm->prev;
  }

  if (root == alarm) {
    root = alarm->next;
    if (root != NULL) {
      libtock_alarm_command_set(root->reference, root->dt);
    }
  }

  alarm->prev = NULL;
  alarm->next = NULL;

}

int libtock_timer_in(uint32_t ms, subscribe_upcall cb, void* ud, tock_timer_t *timer) {
  uint32_t frequency;
  libtock_alarm_command_get_frequency(&frequency);
  uint32_t interval = (ms / 1000) * frequency + (ms % 1000) * (frequency / 1000);
  uint32_t now;
  libtock_alarm_command_read(&now);
  return libtock_alarm_at(now, interval, cb, ud, &timer->alarm);
}

static void alarm_timer_repeating_upcall( uint32_t                     now,
                                          __attribute__ ((unused)) int unused1,
                                          __attribute__ ((unused)) int unused2,
                                          void*                        ud) {
  tock_timer_t* repeating = (tock_timer_t*)ud;
  uint32_t interval       = repeating->interval;
  uint32_t cur_exp        = repeating->alarm.reference + interval;
  libtock_alarm_at(cur_exp, interval, (subscribe_upcall*)alarm_timer_repeating_upcall,
           (void*)repeating, &repeating->alarm);
  repeating->cb(now, cur_exp, 0, repeating->ud);
}

void libtock_timer_every(uint32_t ms, subscribe_upcall cb, void* ud, tock_timer_t* repeating) {
  uint32_t frequency;
  libtock_alarm_command_get_frequency(&frequency);
  uint32_t interval = (ms / 1000) * frequency + (ms % 1000) * (frequency / 1000);

  repeating->interval = interval;
  repeating->cb       = cb;
  repeating->ud       = ud;

  uint32_t now;
  libtock_alarm_command_read(&now);
  libtock_alarm_at(now, interval, (subscribe_upcall*)alarm_timer_repeating_upcall,
           (void*)repeating, &repeating->alarm);
}

void libtock_timer_cancel(tock_timer_t* timer) {
  libtock_alarm_cancel(&timer->alarm);
}

int libtock_gettimeasticks(struct timeval *tv, __attribute__ ((unused)) void *tzvp)
{
  uint32_t frequency, now, seconds, remainder;

  libtock_alarm_command_get_frequency(&frequency);
  libtock_alarm_command_read(&now);

  // The microsecond calculation will overflow in the intermediate scaling of
  // (remainder * 1000) if the remainder is approximately greater than 4e6. Because
  // remainder is calculated as now % frequency, we can define 0 <= remainder < frequency.
  // This implies that the tv_usec must be of type uint64_t if frequency > 4MHz to avoid
  // an overflow from occurring. We check this in the below assertion statement.
  const uint32_t MAX_VALID_FREQ = 4000000;
  assert(frequency < MAX_VALID_FREQ || sizeof(tv->tv_usec) == sizeof(uint64_t));

  // Confirm frequency assumption
  assert(frequency > 0);

  seconds   = now / frequency;
  remainder = now % frequency;

  // NOTE: the drawback to this microsecond calculation is the potential loss of precision
  // when scaling frequency / 1000 (lose 3 degrees of precision). At the time of this
  // implementation (1/31/24), the Tock timer frequency struct provides support for
  // frequencies such as 1KHz, 16KHz, 1MHz, etc. With such frequencies, there is not a loss
  // of precision as the 3 least significant digits do not encode data. The only case of a lose
  // in precision is for the frequency 32.768KHz. In this case, the loss of precision introduces ~1us
  // of error.
  tv->tv_sec  = seconds;
  tv->tv_usec = (remainder * 1000) / (frequency / 1000);

  return 0;
}
