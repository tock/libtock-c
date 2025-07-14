#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_USB_KEYBOARD_HID 0x90005

// Check if this driver exists.
bool libtock_usb_keyboard_hid_driver_exists(void);

// Configure the upcall for send/receive events.
returncode_t libtock_usb_keyboard_hid_set_upcall(subscribe_upcall callback, void* opaque);

// Set the buffer for sending data.
returncode_t libtock_usb_keyboard_hid_set_readwrite_allow_send_buffer(uint8_t* buffer, uint32_t len);

// Command to send the keyboard press to the host.
returncode_t libtock_usb_keyboard_hid_command_send(void);

#ifdef __cplusplus
}
#endif
