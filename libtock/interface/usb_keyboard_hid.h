#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif



// Function signature for send done callbacks.
//
// - `arg1` (`returncode_t`): Status of USB keyboard HID send operation.
typedef void (*usb_keyboard_hid_callback)(returncode_t);

// Set the buffer to the over the USB keyboard HID interface. The callback will
// be triggered when the send has completed.
returncode_t libtock_usb_keyboard_hid_send(uint8_t* buffer, uint32_t len, usb_keyboard_hid_callback cb);




#ifdef __cplusplus
}
#endif