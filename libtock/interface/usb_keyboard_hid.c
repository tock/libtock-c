#include "usb_keyboard_hid.h"
#include "syscalls/usb_keyboard_hid_syscalls.h"

static void usb_keyboard_hid_upcall(__attribute__ ((unused)) int callback_type,
                                    __attribute__ ((unused)) int unused1,
                                    __attribute__ ((unused)) int unused2,
                                    void*                        opaque) {
  libtock_usb_keyboard_hid_callback cb = (libtock_usb_keyboard_hid_callback) opaque;
  cb(RETURNCODE_SUCCESS);
}

bool libtock_usb_keyboard_hid_exists(void) {
  return libtock_usb_keyboard_hid_driver_exists();
}

returncode_t libtock_usb_keyboard_hid_send(uint8_t* buffer, uint32_t len, libtock_usb_keyboard_hid_callback cb) {
  returncode_t err;

  err = libtock_usb_keyboard_hid_set_upcall(usb_keyboard_hid_upcall, (void*) cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_usb_keyboard_hid_set_readwrite_allow_send_buffer(buffer, len);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_usb_keyboard_hid_command_send(); // Sometimes returns ERESERVE (but everything keeps working??)
  return err;
}
