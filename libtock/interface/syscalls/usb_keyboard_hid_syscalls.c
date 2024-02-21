#include "usb_keyboard_hid_syscalls.h"

bool libtock_usb_keyboard_hid_exists(void) {
  return driver_exists(DRIVER_NUM_USBKEYBOARDHID);
}

returncode_t libtock_usb_keyboard_hid_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_USBKEYBOARDHID, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_usb_keyboard_hid_set_readwrite_allow_send_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_USBKEYBOARDHID, 1, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

returncode_t libtock_usb_keyboard_hid_command_send(void) {
  syscall_return_t cval = command(DRIVER_NUM_USBKEYBOARDHID, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
