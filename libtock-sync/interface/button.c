#include <libtock/interface/syscalls/button_syscalls.h>

#include "button.h"

bool libtocksync_button_exists(void) {
  return libtock_button_driver_exists();
}

returncode_t libtocksync_button_count(int* count) {
  return libtock_button_command_count(count);
}

returncode_t libtocksync_button_read(int button_num, int* button_value) {
  return libtock_button_command_read(button_num, button_value);
}

returncode_t libtocksync_button_wait_for_press(int button_num) {
  returncode_t ret;

  ret = libtock_button_command_enable_interrupt(button_num);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Wait for the callback and for the button to be pressed.
  int index;
  bool pressed;
  while (true) {
    ret = libtocksync_button_yield_wait_for(&index, &pressed);
    if (ret != RETURNCODE_SUCCESS) return ret;

    if (pressed && index == button_num) {
      break;
    }
  }

  return RETURNCODE_SUCCESS;
}
