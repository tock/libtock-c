#include "ambient_light_syscalls.h"

bool libtock_ambient_light_driver_exists(void) {
  return driver_exists(DRIVER_NUM_AMBIENT_LIGHT);
}

returncode_t libtock_ambient_light_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_AMBIENT_LIGHT, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_ambient_light_command_start_intensity_reading(void) {
  syscall_return_t cval = command(DRIVER_NUM_AMBIENT_LIGHT, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
