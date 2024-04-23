#include "alarm_timer.h"

static void delay_upcall(__attribute__ ((unused)) int unused0,
                         __attribute__ ((unused)) int unused1,
                         __attribute__ ((unused)) int unused2,
                         void*                        ud) {
  *((bool*)ud) = true;
}

int libtocksync_alarm_delay_ms(uint32_t ms) {
  bool cond = false;
  tock_timer_t timer;
  int rc;

  if ((rc = libtock_timer_in(ms, delay_upcall, &cond, &timer)) != RETURNCODE_SUCCESS) {
    return rc;
  }

  yield_for(&cond);
  return rc;
}

static void yield_for_timeout_upcall(__attribute__ ((unused)) int unused0,
                                     __attribute__ ((unused)) int unused1,
                                     __attribute__ ((unused)) int unused2,
                                     void*                        ud) {
  *((bool*)ud) = true;
}

int libtocksync_alarm_yield_for_with_timeout(bool* cond, uint32_t ms) {
  bool timeout = false;
  tock_timer_t timer;
  libtock_timer_in(ms, yield_for_timeout_upcall, &timeout, &timer);

  while (!*cond) {
    if (timeout) {
      return RETURNCODE_FAIL;
    }

    yield();
  }

  libtock_timer_cancel(&timer);
  return RETURNCODE_SUCCESS;
}
