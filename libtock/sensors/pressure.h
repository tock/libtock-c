#pragma once

#include "../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for pressure data callback.
//
// - `arg1` (`returncode_t`): Status from sampling the sensor.
// - `arg2` (`int`): Pressure reading in hPa.
typedef void (*libtock_pressure_callback)(returncode_t, int);

// Check if the driver exists.
bool libtock_pressure_exists(void);

// Initiate a pressure measurement and call the callback with the reading when
// finished.
returncode_t libtock_pressure_read(libtock_pressure_callback cb);

#ifdef __cplusplus
}
#endif
