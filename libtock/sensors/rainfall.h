#pragma once

#include "../tock.h"
#include "syscalls/rainfall_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for rainfall data callback.
//
// - `arg1` (`int`): Returncode indicating status from sampling the sensor.
// - `arg2` (`uint32_t`): the number of um of rain in the time period specified
typedef void (*libtock_rainfall_callback)(returncode_t, uint32_t);

// Start a rainfall measurement. The reading will be provided via the callback.
returncode_t libtock_rainfall_read(libtock_rainfall_callback cb, int hours);

#ifdef __cplusplus
}
#endif
