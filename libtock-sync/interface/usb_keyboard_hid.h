#pragma once

#include <tock.h>
#include <libtock/interface/usb_keyboard_hid.h>

#ifdef __cplusplus
extern "C" {
#endif

// Send a buffer to the USB HID keyboard.
returncode_t libtocksync_usb_keyboard_hid_send(uint8_t* buffer, uint32_t len);

// Send a letter to emulate a keyboard keypress.
returncode_t libtocksync_usb_keyboard_hid_send_letter(char letter);

// Send a string of letters as keypresses to emulate typing on a keyboard.
returncode_t libtocksync_usb_keyboard_hid_send_string(char* str, int length);

#ifdef __cplusplus
}
#endif
