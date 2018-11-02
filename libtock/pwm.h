#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

// gives direct control to period and on_period values
int pwm_configure(uint8_t channel, uint16_t period, uint16_t on_period);

// configures period/on_period values to achieve desired freq with 50% duty cycle
int pwm_set_frequency(uint8_t channel, uint32_t freq_hz);

// duty cycle is achieved with highest accuracy available, frequency is varialbe
int pwm_set_duty_cycle(uint8_t channel, float duty_cycle_percentage);

#ifdef __cplusplus
}
#endif