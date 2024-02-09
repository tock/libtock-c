#include "tock.h"

#include "button_syscalls.h"

bool button_exists(void) {
  int count;
  int ret;

  ret = button_command_count(&count);
  if (ret != RETURNCODE_SUCCESS) return false;

  return count > 0;
}

returncode_t button_set_upcall(subscribe_upcall callback, void *opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_BUTTON, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t button_command_count(int* count) {
  syscall_return_t cval = command(DRIVER_NUM_BUTTON, 0, 0, 0);
  return tock_command_return_u32_to_returncode(cval, (uint32_t*) count);
}

returncode_t button_command_enable_interrupt(int button_num) {
  syscall_return_t cval = command(DRIVER_NUM_BUTTON, 1, button_num, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t button_command_disable_interrupt(int button_num) {
  syscall_return_t cval = command(DRIVER_NUM_BUTTON, 2, button_num, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t button_command_read(int button_num, int* button_value) {
  syscall_return_t cval = command(DRIVER_NUM_BUTTON, 3, button_num, 0);
  return tock_command_return_u32_to_returncode(cval, (uint32_t*) button_value);
}