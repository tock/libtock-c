#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_NINEDOF 0x60004

// Check if a ninedof sensor exists.
bool libtock_ninedof_exists(void);

// Configure the upcall for the ninedof driver.
returncode_t libtock_ninedof_set_upcall(subscribe_upcall callback, void* opaque);

// Start an accelerometer reading.
returncode_t libtock_ninedof_command_start_accelerometer_reading(void);

// Start a magnetometer reading.
returncode_t libtock_ninedof_command_start_magnetometer_reading(void);

// Start a gyroscope reading.
returncode_t libtock_ninedof_command_start_gyroscope_reading(void);

#ifdef __cplusplus
}
#endif
