#pragma once

#include <libtock/tock.h>
#include <libtock/sensors/humidity.h>

#ifdef __cplusplus
extern "C" {
#endif

// Read the humidity sensor synchronously.
//
// ## Arguments
//
// - `humidity`: Set to the humidity in hundredths of a percent.
//
// ## Return Value
//
// A returncode indicating whether the sensor read was completed successfully.
returncode_t libtocksync_humidity_read(int* humidity);

#ifdef __cplusplus
}
#endif