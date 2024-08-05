#pragma once

#include "../tock.h"
#include "syscalls/distance_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for distance data callback.
//
// - `arg1` (`returncode_t`): Status from sampling the sensor.
// - `arg2` (`int`): Distance reading in milimeters.
typedef void (*libtock_distance_callback)(returncode_t, int);

// Initiate a reading measurement and return results via the `cb`.
returncode_t libtock_distance_read(libtock_distance_callback cb);

#ifdef __cplusplus
}
#endif
