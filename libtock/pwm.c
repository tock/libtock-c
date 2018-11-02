#include "pwm.h"
#include "tock.h"
#include <stdlib.h>

#define DRIVER_NUM_PWM      (0x00DE0005)

enum cmd {
  CONFIGURE = 1,
  SET_FREQ_HZ = 2,
  SET_DUTY = 3,
  NUM_SIGNALS = 4,
};

int pwm_configure(uint8_t channel, uint16_t period, uint16_t on_period){
  return command(DRIVER_NUM_PWM, CONFIGURE, (uint32_t) channel, (uint32_t) period << 16 | on_period);
}

int pwm_set_frequency(uint8_t channel, uint32_t freq_hz){
  return command(DRIVER_NUM_PWM, SET_FREQ_HZ, (uint32_t) channel, freq_hz);
}

int pwm_set_duty_cycle(uint8_t channel, float duty_cycle_percentage){
  return command(DRIVER_NUM_PWM, SET_DUTY, (uint32_t) channel, (uint32_t) duty_cycle_percentage);
}
