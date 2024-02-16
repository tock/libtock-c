#pragma once

#include <libtock/tock.h>
#include <libtock/interface/buzzer.h>

#ifdef __cplusplus
extern "C" {
#endif

// Play a constant tone for a certain amount of time.
//
// This blocks until the tone has played.
//
// ## Arguments
//
// - `frequency_hz`: Frequency of the tone in hertz.
// - `duration_ms`: Length of the tone in milliseconds.
//
// ## Return Value
//
// A returncode indicating whether the tone was played successfully.
returncode_t libtocksync_buzzer_tone(uint32_t frequency_hz, uint32_t duration_ms);

#ifdef __cplusplus
}
#endif