#include "app_state_syscalls.h"

bool libtock_app_state_exists(void) {
  return driver_exists(DRIVER_NUM_APP_STATE);
}

returncode_t libtock_app_state_set_upcall(subscribe_upcall cb, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_APP_STATE, 0, cb, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_app_state_set_readonly_allow(const uint8_t* buf, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_APP_STATE, 0, buf, len);
  return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_app_state_command_save(uint32_t flash_pointer) {
  syscall_return_t cval = command(DRIVER_NUM_APP_STATE, 1, flash_pointer, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
