#include "proximity_syscalls.h"

bool libtock_proximity_driver_exists(void) {
  return driver_exists(DRIVER_NUM_PROXIMITY);
}

returncode_t libtock_proximity_set_upcall(subscribe_upcall upcall, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_PROXIMITY, 0, upcall, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_proximity_command_read(void) {
  syscall_return_t cval = command(DRIVER_NUM_PROXIMITY, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_proximity_command_read_on_interrupt(uint32_t lower_threshold, uint32_t higher_threshold) {
  syscall_return_t cval = command(DRIVER_NUM_PROXIMITY, 2, lower_threshold, higher_threshold);
  return tock_command_return_novalue_to_returncode(cval);
}
