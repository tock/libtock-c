#include "alarm.h"

struct alarm_cb_data {
  bool fired;
};

static struct alarm_cb_data delay_data = { .fired = false };

static void delay_cb(__attribute__ ((unused)) uint32_t now,
                     __attribute__ ((unused)) uint32_t scheduled) {
  delay_data.fired = true;
}

int libtocksync_alarm_delay_ms(uint32_t ms) {
  delay_data.fired = false;
  alarm_t alarm;
  int rc;

  if ((rc = libtock_alarm_in_ms(ms, delay_cb, &alarm)) != RETURNCODE_SUCCESS) {
    return rc;
  }

  yield_for(&delay_data.fired);
  return rc;
}

static struct alarm_cb_data yf_timeout_data = { .fired = false };

static void yf_timeout_cb(__attribute__ ((unused)) uint32_t now,
                          __attribute__ ((unused)) uint32_t scheduled) {
  yf_timeout_data.fired = true;
}

int libtocksync_alarm_yield_for_with_timeout(bool* cond, uint32_t ms) {
  yf_timeout_data.fired = false;
  alarm_t alarm;
  libtock_alarm_in_ms(ms, yf_timeout_cb, &alarm);

  while (!*cond) {
    if (yf_timeout_data.fired) {
      return RETURNCODE_FAIL;
    }

    yield();
  }

  libtock_alarm_cancel(&alarm);
  return RETURNCODE_SUCCESS;
}
