#include "alarm.h"

#include "syscalls/alarm_syscalls.h"

static void alarm_noop_cb(__attribute__ ((unused)) uint32_t now,
                          __attribute__ ((unused)) uint32_t scheduled,
                          __attribute__ ((unused)) void*    opaque) {}

int libtocksync_alarm_delay_ms(uint32_t ms) {
  libtock_alarm_t alarm;
  int rc;

  if ((rc = libtock_alarm_in_ms(ms, alarm_noop_cb, NULL, &alarm)) != RETURNCODE_SUCCESS) {
    return rc;
  }

  return libtocksync_alarm_yield_wait_for();
}

static struct alarm_cb_data {
  bool fired;
} yf_timeout_data = { .fired = false };

static void yf_timeout_cb(__attribute__ ((unused)) uint32_t now,
                          __attribute__ ((unused)) uint32_t scheduled,
                          __attribute__ ((unused)) void*    opaque) {
  yf_timeout_data.fired = true;
}

int libtocksync_alarm_yield_for_with_timeout(bool* cond, uint32_t ms) {
  yf_timeout_data.fired = false;
  libtock_alarm_t alarm;
  libtock_alarm_in_ms(ms, yf_timeout_cb, NULL, &alarm);

  while (!*cond) {
    if (yf_timeout_data.fired) {
      return RETURNCODE_FAIL;
    }

    yield();
  }

  libtock_alarm_ms_cancel(&alarm);
  return RETURNCODE_SUCCESS;
}
