#pragma once

#include <libtock/sensors/rainfall.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Read the rainfall sensor synchronously.
//
// ## Arguments
//
// - `rainfall`: Set to the mms of rainfall in the specified number of hours.
// - `hours`: The number of hours to get the rainfall data from. 1 to 24 hours
//            are valid values.
//
// ## Return Value
//
// A returncode indicating whether the sensor read was completed successfully.
returncode_t libtocksync_rainfall_read(uint32_t* rainfall, int hours);

#ifdef __cplusplus
}
#endif
