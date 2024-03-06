#include "lps25hb_syscalls.h"

bool libtock_lps25hb_exists(void) {
  return driver_exists(DRIVER_NUM_LPS25HB);
}

returncode_t libtock_lps25hb_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_LPS25HB, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_lps25hb_command_get_pressure(void) {
  syscall_return_t cval = command(DRIVER_NUM_LPS25HB, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
