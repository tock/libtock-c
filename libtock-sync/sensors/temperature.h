#pragma once

#include <libtock/tock.h>

#include "syscalls/temperature_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Read the temperature sensor synchronously.
//
// ## Arguments
//
// - `temperature`: Set to the temperature value in hundredths of degrees
//   centigrade.
//
// ## Return Value
//
// A returncode indicating whether the temperature read was completed
// successfully.
returncode_t libtocksync_temperature_read(int* temperature);

#ifdef __cplusplus
}
#endif
