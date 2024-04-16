#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_TEMPERATURE 0x60000

// Check if temperature sensor exists.
bool libtock_temperature_exists(void);

// units: temperature in hundredths of degrees centigrade.

// Set the callback function to be called when the temperature measurement is
// finished.
//
// callback  - pointer to function to be called
// opaque    - pointer to data provided to the callback
returncode_t libtock_temperature_set_upcall(subscribe_upcall callback, void* opaque);

// Initiate an ambient temperature measurement.
returncode_t libtock_temperature_command_read(void);

#ifdef __cplusplus
}
#endif
