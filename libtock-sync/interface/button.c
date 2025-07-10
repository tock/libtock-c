#include "button.h"

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
