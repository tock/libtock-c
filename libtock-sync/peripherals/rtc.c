#include <libtock/peripherals/syscalls/rtc_syscalls.h>

#include "rtc.h"

#include "syscalls/rtc_syscalls.h"

bool libtocksync_rtc_exists(void) {
  return libtock_rtc_driver_exists();
}

returncode_t libtocksync_rtc_get_date(libtock_rtc_date_t* date) {
  returncode_t ret;

  ret = libtock_rtc_command_get_date();
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_rtc_yield_wait_for_get(date);
  return ret;
}

returncode_t libtocksync_rtc_set_date(libtock_rtc_date_t* set_date) {
  returncode_t ret;
  uint32_t date = set_date->year * (1 << 9) + set_date->month * (1 << 5) + set_date->day;
  uint32_t time = set_date->day_of_week *
                  (1 << 17) + set_date->hour * (1 << 12) + set_date->minute * (1 << 6) + set_date->seconds;

  ret = libtock_rtc_command_set_date(date, time);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_rtc_yield_wait_for_set();
  return ret;
}
