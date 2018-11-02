#include <pwm.h>

int main(void) {
  printf("[PWM] Example started\r\n");

  pwm_set_duty_cycle(0, 50.00);
  printf("[PWM] Configure Done\r\n");

  return 0;
}
