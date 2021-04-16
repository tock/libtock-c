#include "button.h"

int button_subscribe(subscribe_upcall callback, void *ud) {
  subscribe_return_t res = subscribe(DRIVER_NUM_BUTTON, 0, callback, ud);
  return tock_subscribe_return_to_returncode(res);
}

int button_count(int* count) {
  syscall_return_t res = command(DRIVER_NUM_BUTTON, 0, 0, 0);
  return tock_command_return_u32_to_returncode(res, (uint32_t*) count);
}

int button_enable_interrupt(int pin_num) {
  syscall_return_t res = command(DRIVER_NUM_BUTTON, 1, pin_num, 0);
  return tock_command_return_novalue_to_returncode(res);
}

int button_disable_interrupt(int pin_num) {
  syscall_return_t res = command(DRIVER_NUM_BUTTON, 2, pin_num, 0);
  return tock_command_return_novalue_to_returncode(res);
}

int button_read(int pin_num, int* pin_value) {
  syscall_return_t res = command(DRIVER_NUM_BUTTON, 3, pin_num, 0);
  return tock_command_return_u32_to_returncode(res, (uint32_t*) pin_value);
}
