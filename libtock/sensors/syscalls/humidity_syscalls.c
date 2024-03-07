#include "humidity_syscalls.h"
#include "tock.h"

bool libtock_humidity_exists(void) {
  return driver_exists(DRIVER_NUM_HUMIDITY);
}

returncode_t libtock_humidity_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_HUMIDITY, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_humidity_command_read(void) {
  syscall_return_t rval = command(DRIVER_NUM_HUMIDITY, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(rval);
}
