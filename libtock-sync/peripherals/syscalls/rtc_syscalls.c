#include "rtc_syscalls.h"

// DateTime codifies Date structure into two u32 numbers:
//   date: year (12 bits) | month (4 bits) | day (5 bits)
//   time: day_of_week (3 bits) | hour (5 bits) | minute (6 bits) | seconds (6 bits)
static void rtc_decode(uint32_t date, uint32_t time, libtock_rtc_date_t* out) {
  out->year  = date % (1 << 21) / (1 << 9);
  out->month = date % (1 << 9) / (1 << 5);
  out->day   = date % (1 << 5);

  out->day_of_week = time % (1 << 20) / (1 << 17);
  out->hour        = time % (1 << 17) / (1 << 12);
  out->minute      = time % (1 << 12) / (1 << 6);
  out->seconds     = time % (1 << 6);
}

returncode_t libtocksync_rtc_yield_wait_for_get(libtock_rtc_date_t* date) {
  yield_waitfor_return_t ywf;
  returncode_t ret;
  ywf = yield_wait_for(DRIVER_NUM_RTC, 0);
  ret = (returncode_t) ywf.data0;
  if (ret != RETURNCODE_SUCCESS) return ret;
  rtc_decode((uint32_t) ywf.data1, (uint32_t) ywf.data2, date);
  return ret;
}

returncode_t libtocksync_rtc_yield_wait_for_set(void) {
  yield_waitfor_return_t ywf;
  ywf = yield_wait_for(DRIVER_NUM_RTC, 0);
  return (returncode_t) ywf.data0;
}
