#include "pressure_syscalls.h"

bool libtock_pressure_driver_exists(void) {
  return driver_exists(DRIVER_NUM_PRESSURE);
}

returncode_t libtock_pressure_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_PRESSURE, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_pressure_command_read(void) {
  syscall_return_t cval = command(DRIVER_NUM_PRESSURE, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
