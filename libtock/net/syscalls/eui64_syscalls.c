#include "eui64_syscalls.h"

bool libtock_eui64_driver_exists(void) {
  return driver_exists(DRIVER_NUM_EUI64);
}

returncode_t libtock_eui64_command_get(uint64_t* eui64) {
  // Issue the command to the kernel
  syscall_return_t cval = command(DRIVER_NUM_EUI64, EUI64_CMD_GETTER, 0, 0);
  return tock_command_return_u64_to_returncode(cval, eui64);
}
