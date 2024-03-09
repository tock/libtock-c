#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_SOUND_PRESSURE 0x60006

// Check if the sound pressure sensor exists.
bool libtock_sound_pressure_exists(void);

// Set the upcall function for the sound pressure sensor.
returncode_t libtock_sound_pressure_set_upcall(subscribe_upcall callback, void* opaque);

// Enable the sound pressure sensor.
returncode_t libtock_sound_pressure_command_enable(void);

// Disable the sound pressure sensor.
returncode_t libtock_sound_pressure_command_disable(void);

// Read the sound pressure sensor.
returncode_t libtock_sound_pressure_command_read(void);

#ifdef __cplusplus
}
#endif
