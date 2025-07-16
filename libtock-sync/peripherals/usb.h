#pragma once

#include <libtock/peripherals/usb.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

bool libtocksync_usb_exists(void);

// Enable and attach the USB and wait until the USB attaches.
returncode_t libtocksync_usb_enable_and_attach(void);

#ifdef __cplusplus
}
#endif
