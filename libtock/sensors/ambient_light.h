#pragma once

#include "tock.h"
#include "syscalls/ambient_light_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for ambient light data callback.
//
// - `arg1` (`returncode_t`): Status from sampling the sensor.
// - `arg2` (`int`): Ambient light reading in lux (lx).
typedef void (*libtock_ambient_light_callback)(returncode_t, int);


// Request an ambient light reading.
//
// The callback will be triggered with the result in lux.
returncode_t libtock_ambient_light_read_intensity(libtock_ambient_light_callback cb);

#ifdef __cplusplus
}
#endif
