#include "button.h"

subscribe_return_t button_subscribe(subscribe_upcall callback, void *ud) {
  return subscribe(DRIVER_NUM_BUTTON, 0, callback, ud);
}

int button_count(void) {
  syscall_return_t res = command(DRIVER_NUM_BUTTON, 0, 0, 0);
  if (res.type != TOCK_SYSCALL_SUCCESS_U32) {
    return -1;
  } else {
    return res.data[0];
  }
}

syscall_return_t button_enable_interrupt(int pin_num) {
  return command(DRIVER_NUM_BUTTON, 1, pin_num, 0);
}

syscall_return_t button_disable_interrupt(int pin_num) {
  return command(DRIVER_NUM_BUTTON, 2, pin_num, 0);
}

int button_read(int pin_num) {
  syscall_return_t res = command(DRIVER_NUM_BUTTON, 3, pin_num, 0);
  if (res.type != TOCK_SYSCALL_SUCCESS_U32) {
    return -1;
  } else {
    return res.data[0];
  }
}

