#pragma once

#include <libtock/sensors/syscalls/sound_pressure_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a sound pressure reading to complete.
returncode_t libtocksync_sound_pressure_yield_wait_for(uint8_t* sound_pressure);

#ifdef __cplusplus
}
#endif
