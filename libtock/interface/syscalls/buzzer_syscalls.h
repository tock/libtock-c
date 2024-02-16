#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_BUZZER 0x90000

// Check if the buzzer system call driver is available on this board.
bool libtock_buzzer_exists(void);

// Subscribe an upcall for the buzzer.
returncode_t libtock_buzzer_set_upcall(subscribe_upcall callback, void* opaque);

// Start a tone at a given frequency for a given duration.
returncode_t libtock_buzzer_command_tone(uint32_t frequency_hz, uint32_t duration_ms);

#ifdef __cplusplus
}
#endif

