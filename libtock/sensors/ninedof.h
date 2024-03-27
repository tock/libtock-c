#pragma once

#include "tock.h"
#include "syscalls/ninedof_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for ninedof data callbacks. This callback is general for
// the different 9-DOF sensors.
//
// - `arg1` (`returncode_t`): Status from sampling the sensor.
// - `arg2` (`int`): Reading in x dimension.
// - `arg3` (`int`): Reading in y dimension.
// - `arg4` (`int`): Reading in z dimension.
typedef void (*libtock_ninedof_callback)(returncode_t, int, int, int);


// Read the accelerometer.
//
// The X, Y, Z and measurements will be returned via the callback.
returncode_t libtock_ninedof_read_accelerometer(libtock_ninedof_callback cb);

// Read the magnetometer.
//
// The X, Y, Z and measurements will be returned via the callback.
returncode_t libtock_ninedof_read_magnetometer(libtock_ninedof_callback cb);

// Read the gyroscope.
//
// The X, Y, Z and measurements will be returned via the callback.
returncode_t libtock_ninedof_read_gyroscope(libtock_ninedof_callback cb);

#ifdef __cplusplus
}
#endif
