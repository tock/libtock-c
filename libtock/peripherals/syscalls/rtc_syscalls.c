#include "rtc_syscalls.h"
#include "tock.h"

bool libtock_rtc_exists(void) {
  return driver_exists(DRIVER_NUM_RTC);
}

returncode_t libtock_rtc_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_RTC, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_rtc_command_get_date(void) {
  syscall_return_t cval = command(DRIVER_NUM_RTC, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_rtc_command_set_date(uint32_t date, uint32_t time) {
  syscall_return_t cval = command(DRIVER_NUM_RTC, 2, date, time);
  return tock_command_return_novalue_to_returncode(cval);
}
