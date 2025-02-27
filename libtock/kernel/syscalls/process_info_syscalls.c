#include "process_info_syscalls.h"

bool libtock_process_info_exists(void) {
  return driver_exists(DRIVER_NUM_PROCESS_INFO);
}

returncode_t libtock_process_info_set_allow_readwrite_info_buffer(uint8_t* buffer, uint32_t length) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_PROCESS_INFO, 0, (void*) buffer, length);
  return tock_allow_rw_return_to_returncode(aval);
}

returncode_t libtock_process_info_command_get_process_count(uint32_t* count) {
  syscall_return_t cval = command(DRIVER_NUM_PROCESS_INFO, 1, 0, 0);
  return tock_command_return_u32_to_returncode(cval, count);
}

returncode_t libtock_process_info_command_get_process_ids(uint32_t* count) {
  syscall_return_t cval = command(DRIVER_NUM_PROCESS_INFO, 2, 0, 0);
  return tock_command_return_u32_to_returncode(cval, count);
}

returncode_t libtock_process_info_command_get_short_ids(uint32_t* count) {
  syscall_return_t cval = command(DRIVER_NUM_PROCESS_INFO, 3, 0, 0);
  return tock_command_return_u32_to_returncode(cval, count);
}

returncode_t libtock_process_info_command_get_process_name(uint32_t process_id) {
  syscall_return_t cval = command(DRIVER_NUM_PROCESS_INFO, 4, process_id, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
