#include "syscalls/usb_syscalls.h"
#include "usb.h"

static void usb_upcall(int                         status,
                       __attribute__((unused)) int v1,
                       __attribute__((unused)) int v2,
                       void*                       opaque) {
  libtock_usb_callback_attached cb = (libtock_usb_callback_attached) opaque;
  cb(tock_status_to_returncode(status));
}

bool libtock_usb_exists(void) {
  return libtock_usb_driver_exists();
}

returncode_t libtock_usb_enable_and_attach(libtock_usb_callback_attached cb) {
  returncode_t err;

  err = libtock_usb_set_upcall(usb_upcall, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_usb_command_enable_and_attach();
  return err;
}
