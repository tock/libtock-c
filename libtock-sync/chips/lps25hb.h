#pragma once

#include <tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a pressure reading.
//
// ## Arguments
//
// - `pressure`: Set to the pressure reading.
//
// ## Return Value
//
// A returncode indicating whether the pressure read was completed
// successfully.
returncode_t libtocksync_lps25hb_get_pressure(int* pressure);

#ifdef __cplusplus
}
#endif
