#include "read_only_state_syscalls.h"

bool libtock_read_only_state_exists(void) {
  return driver_exists(DRIVER_NUM_READ_ONLY_STATE);
}

returncode_t libtock_read_only_state_set_userspace_read_allow_allocate_region(uint8_t* buffer, uint32_t length) {
  allow_userspace_r_return_t aval = allow_userspace_read(DRIVER_NUM_READ_ONLY_STATE, 0, (void*) buffer, length);
  return tock_allow_userspace_r_return_to_returncode(aval);
}

returncode_t libtock_read_only_state_command_get_version(uint32_t* version) {
  syscall_return_t cval = command(DRIVER_NUM_READ_ONLY_STATE, 1, 0, 0);
  return tock_command_return_u32_to_returncode(cval, version);
}
