#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_PRESSURE 0x60008

// Check if the pressure driver exists.
bool libtock_pressure_driver_exists(void);

// Set the upcall for the pressure sensor.
returncode_t libtock_pressure_set_upcall(subscribe_upcall callback, void* opaque);

// Read the pressure sensor.
returncode_t libtock_pressure_command_read(void);

#ifdef __cplusplus
}
#endif
