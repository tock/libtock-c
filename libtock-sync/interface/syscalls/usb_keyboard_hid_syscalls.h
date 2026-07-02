#pragma once

#include <libtock/interface/syscalls/usb_keyboard_hid_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a USB keyboard HID send upcall.
returncode_t libtocksync_usb_keyboard_hid_yield_wait_for(void);

#ifdef __cplusplus
}
#endif
