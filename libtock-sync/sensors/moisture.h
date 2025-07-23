#pragma once

#include <libtock/sensors/moisture.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Read the moisture sensor synchronously.
//
// ## Arguments
//
// - `moisture`: Set to the moisture in hundredths of a percent.
//
// ## Return Value
//
// A returncode indicating whether the sensor read was completed successfully.
returncode_t libtocksync_moisture_read(int* moisture);

#ifdef __cplusplus
}
#endif
