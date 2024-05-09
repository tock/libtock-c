#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_USB 0x20005

// Check if the USB syscall driver exists.
bool libtock_usb_exists(void);

// Set the USB upcall.
returncode_t libtock_usb_set_upcall(subscribe_upcall upcall, void *opaque);

// Enable the USB connection.
returncode_t libtock_usb_command_enable_and_attach(void);

#ifdef __cplusplus
}
#endif

