#include "rainfall_syscalls.h"

bool libtock_rainfall_driver_exists(void) {
  return driver_exists(DRIVER_NUM_RAINFALL);
}

returncode_t libtock_rainfall_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_RAINFALL, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_rainfall_command_read(int hours) {
  syscall_return_t rval = command(DRIVER_NUM_RAINFALL, 1, hours, 0);
  return tock_command_return_novalue_to_returncode(rval);
}
