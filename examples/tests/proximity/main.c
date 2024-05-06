#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <libtock-sync/sensors/proximity.h>
#include <libtock/interface/led.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/tock.h>

int main(void) {
  returncode_t ret;
  printf("[TEST] Proximity Sensor\n");

  // Check if driver/sensor is on the board
  if (!libtock_proximity_exists()) {
    printf("Proximity sensor does not exist\n");
    return -1;
  }

  // Check led count
  int num_leds;
  libtock_led_count(&num_leds);
  printf("Number of LEDs on the board: %d\n", num_leds);

  // Blink LED lights faster as proximity reading increases.
  // Main Loop starts once proximity reading is above a certain threshold (175)

  uint8_t frequency = 255;
  int period        = 1000;

  ret = libtocksync_proximity_read_on_interrupt(0, 175, &frequency);
  if (ret != RETURNCODE_SUCCESS) {
    printf("Unable to setup proximity interrupts\n");
    return -2;
  }

  while (true) {
    for (int led = 0; led < num_leds; led++) {
      libtock_led_on(led);
      libtocksync_alarm_delay_ms(period / (frequency + 1));
      libtock_led_off(led);
    }

    ret = libtocksync_proximity_read(&frequency);
    if (ret != RETURNCODE_SUCCESS) {
      printf("Could not read proximity");
      return -3;
    }
  }
}
