#pragma once

#include <libtock/peripherals/syscalls/usb_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a USB operation to complete.
returncode_t libtocksync_usb_yield_wait_for(void);

#ifdef __cplusplus
}
#endif
