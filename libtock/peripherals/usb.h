#pragma once

#include "tock.h"
#include "syscalls/usb_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for USB callback.
//
// - `arg1` (`returncode_t`): Status from attaching USB. SUCCESS if all inputs
//   are valid, else EINVAL.
typedef void (*libtock_usb_callback_attached)(returncode_t);

// Enable the USB controller and attach to the bus.
//
// Returns `EINVAL` if usb_subscribe() has not previously been called to
// register a callback. Returns `SUCCESS` if the callback is present and will be
// used to report completion of this operation.
returncode_t libtock_usb_enable_and_attach(libtock_usb_callback_attached cb);

#ifdef __cplusplus
}
#endif
