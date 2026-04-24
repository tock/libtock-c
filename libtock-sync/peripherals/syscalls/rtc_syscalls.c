#include "rtc_syscalls.h"

// DateTime codifies Date structure into two u32 numbers:
//   date: year (12 bits) | month (4 bits) | day (5 bits)
//   time: day_of_week (3 bits) | hour (5 bits) | minute (6 bits) | seconds (6 bits)
static void rtc_decode(uint32_t date, uint32_t time, libtock_rtc_date_t* out) {
  out->year  = (date >> 9) & 0xFFF;
  out->month = (date >> 5) & 0xF;
  out->day   = date & 0x1F;

  out->day_of_week = (time >> 17) & 0x7;
  out->hour        = (time >> 12) & 0x1F;
  out->minute      = (time >> 6) & 0x3F;
  out->seconds     = time & 0x3F;
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
