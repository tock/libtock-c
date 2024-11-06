#include "distance_syscalls.h"

bool libtock_distance_exists(void) {
  return driver_exists(DRIVER_NUM_DISTANCE);
}

returncode_t libtock_distance_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_DISTANCE, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_distance_command_read(void) {
  syscall_return_t cval = command(DRIVER_NUM_DISTANCE, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int libtock_distance_get_minimum_distance(void) {
  return command(DRIVER_NUM_DISTANCE, 2, 0, 0).data[0];
}

int libtock_distance_get_maximum_distance(void) {
  return command(DRIVER_NUM_DISTANCE, 3, 0, 0).data[0];
}