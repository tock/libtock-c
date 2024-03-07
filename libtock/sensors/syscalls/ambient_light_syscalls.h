#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_AMBIENT_LIGHT 0x60002

// Check if the ambient light driver exists.
bool libtock_ambient_light_exists(void);

// Configure the upcall when the reading is ready.
returncode_t libtock_ambient_light_set_upcall(subscribe_upcall callback, void* opaque);

// Request a sensor reading.
returncode_t libtock_ambient_light_command_start_intensity_reading(void);

#ifdef __cplusplus
}
#endif
