#include "usb_syscalls.h"

bool libtock_usb_exists(void) {
  return driver_exists(DRIVER_NUM_USB);
}

returncode_t libtock_usb_set_upcall(subscribe_upcall upcall, void *opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_USB, 0, upcall, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_usb_command_enable_and_attach(void) {
  syscall_return_t cval = command(DRIVER_NUM_USB, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
