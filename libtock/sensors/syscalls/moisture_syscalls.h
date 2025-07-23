#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_MOISTURE 0x6000A

// Check if the moisture driver is installed.
bool libtock_moisture_exists(void);

// Configure the upcall for when the reading is ready.
returncode_t libtock_moisture_set_upcall(subscribe_upcall callback, void* opaque);

// Read the sensor.
returncode_t libtock_moisture_command_read(void);

#ifdef __cplusplus
}
#endif
