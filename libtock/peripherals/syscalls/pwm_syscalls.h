#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_PWM 0x00010

// Pack a pin number and duty cycle into the single `data1` argument used by
// the start command. The pin occupies the low 16 bits and the duty cycle
// occupies the high 16 bits.
#define PWM_PIN_DUTYCYCLE(pin, duty_cycle) ((((uint32_t) (duty_cycle) & 0xFFFF) << 16) | ((pin) & 0xFFFF))

// Check if the PWM driver exists.
bool libtock_pwm_driver_exists(void);

// Start the PWM output on `pin` at `frequency_hz` with the given
// `duty_cycle`.
//
// `duty_cycle` is a percentage with 2 decimal places (e.g. 6050 represents
// a duty cycle of 60.50%). The maximum value is 10000 (100.00%).
returncode_t libtock_pwm_command_start(uint16_t pin, uint16_t duty_cycle, uint32_t frequency_hz);

// Stop the PWM output on `pin`.
returncode_t libtock_pwm_command_stop(uint16_t pin);

// Get the maximum frequency, in hertz, supported by `pin`.
returncode_t libtock_pwm_command_get_max_frequency(uint16_t pin, uint32_t* frequency_hz);

// Get the number of PWM pins available on the board.
returncode_t libtock_pwm_command_get_count(uint32_t* count);

#ifdef __cplusplus
}
#endif
