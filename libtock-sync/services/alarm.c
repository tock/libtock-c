#include "alarm.h"

// Declare non-public export of helper from libtock.
uint32_t _ms_to_ticks(uint32_t ms);

struct alarm_cb_data {
  bool fired;
};

int libtocksync_alarm_delay_ms(uint32_t ms) {
  int rc;
  uint32_t ticks = _ms_to_ticks(ms);
  if ((rc = libtock_alarm_command_set_relative_blind(ticks)) != RETURNCODE_SUCCESS) {
    return rc;
  }

  yield_waitfor_return_t yval = yield_wait_for(DRIVER_NUM_ALARM, 1);
  if (yval.data0 != RETURNCODE_SUCCESS) return yval.data0;

  return rc;
}

static struct alarm_cb_data yf_timeout_data = { .fired = false };

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
