#include <libtock/interface/syscalls/usb_keyboard_hid_syscalls.h>

#include "usb_keyboard_hid_syscalls.h"

returncode_t libtocksync_usb_keyboard_hid_yield_wait_for(void) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_USB_KEYBOARD_HID, 0);

  return (returncode_t) ret.data0;
}
