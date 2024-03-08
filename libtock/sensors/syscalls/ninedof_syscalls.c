#include "ninedof.h"
#include "ninedof_syscalls.h"
#include "tock.h"

bool libtock_ninedof_exists(void) {
  return driver_exists(DRIVER_NUM_NINEDOF);
}

returncode_t libtock_ninedof_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_NINEDOF, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_ninedof_command_start_accelerometer_reading(void) {
  syscall_return_t cval = command(DRIVER_NUM_NINEDOF, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_ninedof_command_start_magnetometer_reading(void) {
  syscall_return_t cval = command(DRIVER_NUM_NINEDOF, 100, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_ninedof_command_start_gyroscope_reading(void) {
  syscall_return_t cval = command(DRIVER_NUM_NINEDOF, 200, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
