#include <inttypes.h>
#include <stdio.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/interface/console.h>
#include <libtock/peripherals/pwm.h>
#include <libtock/tock.h>

// Frequency to drive each PWM pin at, in hertz.
#define PWM_FREQUENCY_HZ 1000

// Number of duty-cycle steps in one full pulse (off -> full brightness ->
// off).
#define PULSE_STEPS 50

// Delay between duty-cycle updates, in milliseconds.
#define STEP_DELAY_MS 50

// Compute the duty cycle for a given step of a triangle wave that ramps up
// over the first half of the pulse and back down over the second half.
static uint16_t triangle_duty_cycle(uint32_t step) {
  uint32_t half  = PULSE_STEPS / 2;
  uint32_t phase = step % PULSE_STEPS;
  uint32_t level = (phase < half) ? phase : (PULSE_STEPS - phase);
  return (uint16_t) (level * 10000 / half);
}

int main(void) {
  returncode_t ret;
  uint32_t count;

  printf("[PWM-LED-PULSE] Pulse all PWM pins\n");

  if (!libtock_pwm_exists()) {
    printf("[PWM-LED-PULSE] ERROR: No PWM driver\n");
    return -1;
  }

  ret = libtock_pwm_get_count(&count);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[PWM-LED-PULSE] ERROR: Could not get PWM pin count: %s\n", tock_strrcode(ret));
    return -1;
  }

  printf("[PWM-LED-PULSE] Found %" PRIu32 " PWM pin(s)\n", count);

  if (count == 0) {
    printf("[PWM-LED-PULSE] ERROR: No PWM pins available on this board\n");
    return -1;
  }

  // Offset each pin's starting phase within the pulse so pins do not all
  // pulse in lockstep.
  uint32_t phase_offset = PULSE_STEPS / count;

  uint32_t step = 0;
  while (1) {
    for (uint32_t pin = 0; pin < count; pin++) {
      uint16_t duty_cycle = triangle_duty_cycle(step + (pin * phase_offset));

      ret = libtock_pwm_start((uint16_t) pin, duty_cycle, PWM_FREQUENCY_HZ);
      if (ret != RETURNCODE_SUCCESS) {
        printf("[ERROR] Could not start PWM on pin %" PRIu32 ": %s\n", pin, tock_strrcode(ret));
        return -1;
      }
    }

    libtocksync_alarm_delay_ms(STEP_DELAY_MS);
    step++;
  }

  return 0;
}
