#pragma once

#include <libtock/sensors/ambient_light.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Read the ambient light sensor synchronously.
//
// ## Arguments
//
// - `lux_value`: Set to the light level in lux.
//
// ## Return Value
//
// A returncode indicating whether the sensor read was completed successfully.
returncode_t libtocksync_ambient_light_read_intensity(int* lux_value);

#ifdef __cplusplus
}
#endif