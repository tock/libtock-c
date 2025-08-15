#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_RAINFALL 0x6000B

// Check if the rainfall driver is installed.
bool libtock_rainfall_driver_exists(void);

// Configure the upcall for when the reading is ready.
returncode_t libtock_rainfall_set_upcall(subscribe_upcall callback, void* opaque);

// Read the sensor.
returncode_t libtock_rainfall_command_read(int hours);

#ifdef __cplusplus
}
#endif
