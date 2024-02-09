#include "tock.h"

#include "temperature_syscalls.h"

bool temperature_exists(void) {
  return driver_exists(DRIVER_NUM_TEMPERATURE);
}

returncode_t temperature_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_TEMPERATURE, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t temperature_command_read(void) {
  syscall_return_t cval = command(DRIVER_NUM_TEMPERATURE, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
