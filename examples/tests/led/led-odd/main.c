#include <libtock/interface/led.h>

int main(void) {
  // Ask the kernel how many LEDs are on this board.
  int num_leds;
  int err = libtock_led_count(&num_leds);
  if (err < 0) return err;

  // Turn on all odd LEDs and off all even LEDs.
  for (int i = 0; i < num_leds; i++) {
    if (i % 2 != 0) {
      libtock_led_on(i);
    } else {
      libtock_led_off(i);
    }
  }
}
