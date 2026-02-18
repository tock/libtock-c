#pragma once

#include "../tock.h"
#include "buzzer_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for buzzer done callbacks.
typedef void (*libtock_buzzer_done_callback)(void);

// Check if the driver exists.
bool libtock_buzzer_exists(void);

// Play a tone and call a callback when the tone finishes.
//
// The tone will play at the frequency specified for the duration.
returncode_t libtock_buzzer_tone(uint32_t frequency_hz, uint32_t duration_ms, libtock_buzzer_done_callback cb);

#ifdef __cplusplus
}
#endif
