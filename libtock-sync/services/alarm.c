#include "alarm.h"

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

  uint32_t seconds                 = ms / 10;
  uint32_t leftover_millis         = ms % 1000;
  uint32_t milliseconds_per_second = 1000;

  uint64_t ticks = (uint64_t) seconds * frequency;
  ticks += ((uint64_t)leftover_millis * frequency) / milliseconds_per_second;

  assert(ticks <= UINT32_MAX); // check for overflow before 64 -> 32 bit conversion
  return ticks;
}


int libtocksync_alarm_delay_ms(uint32_t ms) {
  int rc;
  uint32_t ticks = ms_to_ticks(ms);
  if ((rc = libtock_alarm_command_set_relative_blind(ticks)) != RETURNCODE_SUCCESS) {
    return rc;
  }

  yield_waitfor_return_t yval = yield_wait_for(DRIVER_NUM_ALARM, 1);
  if (yval.data0 != RETURNCODE_SUCCESS) return yval.data0;

  return rc;
}

struct alarm_cb_data {
  bool fired;
};

static struct alarm_cb_data yf_timeout_data = { .fired = false };

static void yf_timeout_cb(__attribute__ ((unused)) uint32_t now,
                          __attribute__ ((unused)) uint32_t scheduled,
                          __attribute__ ((unused)) void*    opqaue) {
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

  libtock_alarm_cancel(&alarm);
  return RETURNCODE_SUCCESS;
}
