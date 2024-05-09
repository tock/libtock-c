#pragma once

#include "../tock.h"
#include "syscalls/sound_pressure_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for sound pressure data callback.
//
// - `arg1` (`returncode_t`): Status from sampling the sensor.
// - `arg2` (`uint8_t`): Sound pressure reading in dB.
typedef void (*libtock_sound_pressure_callback)(returncode_t, uint8_t);

// Initiate an ambient sound pressure measurement.
//
// The sound pressure reading will be returned via the callback.
returncode_t libtock_sound_pressure_read(libtock_sound_pressure_callback cb);

#ifdef __cplusplus
}
#endif
