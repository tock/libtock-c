#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_HUMIDITY 0x60001

// Check if the humidity driver is installed.
bool libtock_humidity_exists(void);

// Configure the upcall for when the reading is ready.
returncode_t libtock_humidity_set_upcall(subscribe_upcall callback, void* opaque);

// Read the sensor.
returncode_t libtock_humidity_command_read(void);

#ifdef __cplusplus
}
#endif
