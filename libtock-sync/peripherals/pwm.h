#pragma once

#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

bool libtocksync_pwm_exists(void);

// Start the PWM output on `pin` at `frequency_hz` with the given
// `duty_cycle`.
//
// `duty_cycle` is a percentage with 2 decimal places (e.g. 6050 represents
// a duty cycle of 60.50%). The maximum value is 10000 (100.00%), which is
// the maximum duty cycle supported by the pin.
returncode_t libtocksync_pwm_start(uint16_t pin, uint16_t duty_cycle, uint32_t frequency_hz);

// Stop the PWM output on `pin`.
returncode_t libtocksync_pwm_stop(uint16_t pin);

// Get the maximum frequency, in hertz, supported by `pin`.
returncode_t libtocksync_pwm_get_max_frequency(uint16_t pin, uint32_t* frequency_hz);

// Get the number of PWM pins available on the board.
returncode_t libtocksync_pwm_get_count(uint32_t* count);

#ifdef __cplusplus
}
#endif
