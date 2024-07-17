/* vim: set sw=2 expandtab tw=80: */

/* This application can operate in three modes: input, output
 * or interrupt. The mode is set as a constant in main().
 *   - Output mode uses userspace GPIO pin 0 and toggles it up and down.
 *   - Input mode uses userspace GPIO pin 0 (the 0th pin made available
 *   to userspace programs. Consult the boot sequence of your board or
 *   its documentation to determine which hardware pin this is.
 *   - Interrupt mode uses userspace GPIO pin 0 (see input mode above).
 *   It executes a callback when the pin goes from low to high. To test
 *   this, connect the pin to Vdd.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/interface/console.h>
#include <libtock/interface/led.h>
#include <libtock/peripherals/gpio.h>
#include <libtock/tock.h>

struct alarm_cb_data {
  bool fired;
  int callback_count;
};

static struct alarm_cb_data data = { .fired = false, .callback_count = 0 };

static void alarm_cb(__attribute__ ((unused)) uint32_t now,
                     __attribute__ ((unused)) uint32_t scheduled,
                     __attribute__ ((unused)) void*    opaque) {
  data.callback_count = data.callback_count + 1;
  data.fired = true;
}

// **************************************************
// GPIO output example: toggles pin
// **************************************************
static void gpio_output(void) {
  printf("Periodically toggling pin\n");

  libtock_gpio_enable_output(0);
  // Start repeating alarm
  data.fired = false;
  libtock_alarm_t alarm_repeating;
  libtock_alarm_repeating_every_ms(1000, alarm_cb, NULL, &alarm_repeating);

  while (1) {
    yield_for(&data.fired);
    data.fired = false;
    libtock_gpio_toggle(0);
  }
}

// **************************************************
// GPIO input example: reads userspace pin 0.
// **************************************************
static void gpio_input(void) {
  printf("Periodically reading value of the GPIO 0 pin\n");
  printf("Jump pin high to test (defaults to low)\n");

  // set userspace pin 0 as input and start repeating alarm
  // pin is configured with a pull-down resistor, so it should read 0 as default
  libtock_gpio_enable_input(0, libtock_pull_down);
  libtock_alarm_t alarm_repeating;
  data.fired = false;
  libtock_alarm_repeating_every_ms(500, alarm_cb, NULL, &alarm_repeating);

  while (1) {
    // print pin value
    int pin_val;
    libtock_gpio_read(0, &pin_val);
    printf("\tValue(%d)\n", pin_val);
    yield_for(&data.fired);
    data.fired = 0;
  }
}

// **************************************************
// GPIO interrupt example
// **************************************************
static bool gpio_interrupt_fired = false;
static void gpio_cb(__attribute__ ((unused)) uint32_t pin_num,
                    __attribute__ ((unused)) bool     arg2) {
  gpio_interrupt_fired = true;
}

static void gpio_interrupt(void) {
  printf("Print userspace GPIO 0 pin reading whenever its value changes\n");
  printf("Jump pin high to test\n");

  // set callback for GPIO interrupts
  libtock_gpio_set_interrupt_callback(gpio_cb);

  // set userspace pin 0 as input and enable interrupts on it
  libtock_gpio_enable_input(0, libtock_pull_down);
  libtock_gpio_enable_interrupt(0, libtock_change);

  gpio_interrupt_fired = false;
  while (1) {
    yield_for(&gpio_interrupt_fired);
    gpio_interrupt_fired = false;
    printf("\tGPIO Interrupt!\n");
  }
}


int main(void) {
  printf("*********************\n");
  printf("GPIO Test Application\n");

  // Set mode to which test you want
  uint8_t mode = 1;

  switch (mode) {
    case 0: gpio_interrupt();
      break;
    case 1: gpio_output();
      break;
    case 2: gpio_input();
      break;
  }
  return 0;
}
