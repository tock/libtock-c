#include <limits.h>
#include <stdio.h>

#include <libtock/interface/led.h>
#include <libtock-sync/sensors/ninedof.h>

int main(void) {
  printf("[App] Accelerometer -> LEDs\n");

  while (1) {
    int x, y, z;
    libtocksync_ninedof_read_accelerometer(&x, &y, &z);

    // abs()
    if (x < 0) x *= -1;
    if (y < 0) y *= -1;
    if (z < 0) z *= -1;

    // Set LEDs based on acceleration.
    int largest = INT_MIN;
    if (x > largest) largest = x;
    if (y > largest) largest = y;
    if (z > largest) largest = z;

    if (x == largest) {
      libtock_led_on(0);
    } else {
      libtock_led_off(0);
    }
    if (y == largest) {
      libtock_led_on(1);
    } else {
      libtock_led_off(1);
    }
    if (z == largest) {
      libtock_led_on(2);
    } else {
      libtock_led_off(2);
    }
  }

  return 0;
}
