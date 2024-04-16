/* vim: set sw=2 expandtab tw=80: */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <libtock/interface/console.h>
#include <libtock/interface/led.h>
#include <libtock/peripherals/gpio.h>
#include <libtock/tock.h>

typedef struct {
  bool pir;
  uint8_t reed_switch;
} SensorData_t;

static SensorData_t sensor_data = {
  .pir         = 0,
  .reed_switch = 0,
};

// callback for gpio interrupts
static void gpio_cb (uint32_t pin_num,
                     bool     pin_val) {

  // save sensor data
  if (pin_num == 1) {
    // interrupt from pir sensor
    sensor_data.pir = pin_val;

  } else if (pin_num == 2) {
    // interrupt from reed switch
    sensor_data.reed_switch = pin_val;
  }
}

// This application reads from multiple sources:
//  * GPIO input from PIR sensor (motion)
//  * GPIO input from Hall-effect sensor (door open/close)
//  * Accelerometer (movement)
//  and makes that available over RF communication
int main(void) {
  printf("*********************\n");
  printf("Security Application\n");

  // configure pins
  libtock_gpio_set_interrupt_callback(gpio_cb);
  libtock_gpio_enable_input(0, libtock_pull_none);
  libtock_gpio_enable_interrupt(0, libtock_change);
  libtock_gpio_enable_input(1, libtock_pull_up);
  libtock_gpio_enable_interrupt(1, libtock_change);

  // configure accelerometer
  // TODO

  // configure radio
  // TODO

  while (1) {
    yield();
    libtock_led_toggle(0);

    printf("\tPIR:\t\t%d\n\tReed Switch:\t%d\n\n",
           sensor_data.pir, sensor_data.reed_switch);
  }

  return 0;
}
