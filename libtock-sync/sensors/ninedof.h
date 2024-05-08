#pragma once

#include <libtock/sensors/ninedof.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Read the accelerometer synchronously.
//
// ## Arguments
//
// - `x`: Set to the X acceleration component.
// - `y`: Set to the Y acceleration component.
// - `z`: Set to the Z acceleration component.
//
// ## Return Value
//
// A returncode indicating whether the read was completed successfully.
returncode_t libtocksync_ninedof_read_accelerometer(int* x, int* y, int* z);

// Read the accelerometer and calculate the accelerometer magnitude.
//
// ## Arguments
//
// - `magnetometer`: Set to the magnitude of the acceleration.
//
// ## Return Value
//
// A returncode indicating whether the read was completed successfully.
returncode_t libtocksync_ninedof_read_accelerometer_magnitude(double* magnitude);

// Read the magnetometer synchronously.
//
// ## Arguments
//
// - `x`: Set to the X magnetometer component.
// - `y`: Set to the Y magnetometer component.
// - `z`: Set to the Z magnetometer component.
//
// ## Return Value
//
// A returncode indicating whether the read was completed successfully.
returncode_t libtocksync_ninedof_read_magnetometer(int* x, int* y, int* z);

// Read the gyroscope synchronously.
//
// ## Arguments
//
// - `x`: Set to the X component.
// - `y`: Set to the Y component.
// - `z`: Set to the Z component.
//
// ## Return Value
//
// A returncode indicating whether the read was completed successfully.
returncode_t libtocksync_ninedof_read_gyroscope(int* x, int* y, int* z);

#ifdef __cplusplus
}
#endif