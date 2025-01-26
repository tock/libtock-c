#pragma once

#include "../tock.h"
#include "syscalls/moisture_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for moisture data callback.
//
// - `arg1` (`int`): Returncode indicating status from sampling the sensor.
// - `arg2` (`int`): moisture in hundredths of percent.
typedef void (*libtock_moisture_callback)(returncode_t, int);

// Start a moisture measurement. The reading will be provided via the callback.
returncode_t libtock_moisture_read(libtock_moisture_callback cb);

#ifdef __cplusplus
}
#endif
