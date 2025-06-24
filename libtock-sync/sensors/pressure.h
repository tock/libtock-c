#pragma once

#include <libtock/sensors/pressure.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Read the pressure synchronously.
//
// ## Arguments
//
// - `pressure`: The pressure reading in hPa.
//
// ## Return Value
//
// A returncode indicating whether the read was completed successfully.
returncode_t libtocksync_pressure_read(int* pressure);

#ifdef __cplusplus
}
#endif
