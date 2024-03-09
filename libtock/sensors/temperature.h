#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif



// Function signature for temperature data callback.
//
// - `arg1` (`int`): Returncode indicating status from sampling the sensor.
// - `arg2` (`int`): Temperature reading in hundredths of degrees centigrade.
typedef void (*libtock_temperature_callback)(returncode_t, int);



// Initiate an ambient temperature measurement and return results via the `cb`.
returncode_t libtock_temperature_read(libtock_temperature_callback cb);



#ifdef __cplusplus
}
#endif
