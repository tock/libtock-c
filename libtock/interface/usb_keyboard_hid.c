#include "syscalls/usb_keyboard_hid_syscalls.h"
#include "usb_keyboard_hid.h"



static void usb_keyboard_hid_upcall(__attribute__ ((unused)) int callback_type,
                                    __attribute__ ((unused)) int unused1,
                                    __attribute__ ((unused)) int unused2,
                                    void*                        opaque) {
  usb_keyboard_hid_callback cb = (usb_keyboard_hid_callback) opaque;
  cb(RETURNCODE_SUCCESS);
}

returncode_t libtock_usb_keyboard_hid_send(uint8_t* buffer, uint32_t len, usb_keyboard_hid_callback cb) {
  returncode_t err;

  err = libtock_usb_keyboard_hid_set_upcall(usb_keyboard_hid_upcall, (void*) cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_usb_keyboard_hid_set_readwrite_allow_send_buffer(buffer, len);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_usb_keyboard_hid_command_send(); // Sometimes returns ERESERVE (but everything keeps working??)
  return err;
}
