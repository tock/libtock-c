#include <math.h>
#include <stdio.h>

#include <libtock-sync/sensors/ninedof.h>
#include <libtock/interface/led.h>

int main(void) {
  int x, y, z;
  int err;

  // Choose the LED to use. We want green (which is usually
  // second in RGB), but will take anything.
  int led = 0;
  int num_leds;
  err = libtock_led_count(&num_leds);
  if (err < 0) {
    printf("No LEDs on this board.\n");
    return err;
  }
  if (num_leds > 1) led = 1;

  while (1) {
    err = libtocksync_ninedof_read_magnetometer(&x, &y, &z);
    if (err < 0) {
      printf("No magnetometer on this board.\n");
      return err;
    }
    printf("x: %d, y: %d, z: %d\n", x, y, z);

    // Compute the X-Y angle of the board.
    double angle = atan2((double) y, (double) x);
    if (y > 0) {
      angle = 90 - angle * (180 / M_PI);
    } else {
      angle = 270 - angle * (180 / M_PI);
    }

    // Turn the LED on if the board is pointing in a certain range.
    if (angle > 50 && angle < 310) {
      libtock_led_off(led);
    } else {
      libtock_led_on(led);
    }
  }

  return 0;
}
