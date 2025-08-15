#pragma once

#include <libtock/sensors/sound_pressure.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

bool libtocksync_sound_pressure_exists(void);

// Enable the sound pressure sensor.
returncode_t libtocksync_sound_pressure_enable(void);

// Disable the sound pressure sensor.
returncode_t libtocksync_sound_pressure_disable(void);

// Read the ambient sound pressure level synchronously.
//
// ## Arguments
//
// - `sound_pressure`: The pressure reading in dB.
//
// ## Return Value
//
// A returncode indicating whether the read was completed successfully.
returncode_t libtocksync_sound_pressure_read(uint8_t* sound_pressure);

#ifdef __cplusplus
}
#endif
