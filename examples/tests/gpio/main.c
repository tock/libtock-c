/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <console.h>
#include <gpio.h>
#include <led.h>
#include <timer.h>
#include <tock.h>

int callback_count = 0;
// callback for timers
static void timer_cb (__attribute__ ((unused)) int arg0,
                      __attribute__ ((unused)) int arg1,
                      __attribute__ ((unused)) int arg2,
                      __attribute__ ((unused)) void* userdata) {
  callback_count = callback_count + 1;
  *((bool*)userdata) = 1;
}

// **************************************************
// GPIO output example
// **************************************************
static void gpio_output(void) {
  printf("Periodically blinking LED\n");

  // Start repeating timer
  static bool resume = 0;
  tock_timer_t timer;
  timer_every(1000, timer_cb, &resume, &timer);

  while (1) {
    yield_for(&resume);
    resume = 0; 
    led_toggle(0);
  }
}

// **************************************************
// GPIO input example
// **************************************************
static void gpio_input(void) {
  printf("Periodically reading value of the GPIO 0 pin\n");
  printf("Jump pin high to test (defaults to low)\n");

  // set LED pin as input and start repeating timer
  // pin is configured with a pull-down resistor, so it should read 0 as default
  gpio_enable_input(0, PullDown);
  tock_timer_t timer;
  static bool resume = 0;
  timer_every(500, timer_cb, &resume, &timer);

  while (1) {
    // print pin value
    int pin_val = gpio_read(0);
    printf("\tValue(%d)\n", pin_val);
    yield_for(&resume);
    resume = 0;
  }
}

// **************************************************
// GPIO interrupt example
// **************************************************
static void gpio_cb (__attribute__ ((unused)) int pin_num,
                     __attribute__ ((unused)) int arg2,
                     __attribute__ ((unused)) int arg3,
                     __attribute__ ((unused)) void* userdata) {}

static void gpio_interrupt(void) {
  printf("Print GPIO 0 pin reading whenever its value changes\n");
  printf("Jump pin high to test\n");

  // set callback for GPIO interrupts
  static bool resume = 0;
  gpio_interrupt_callback(gpio_cb, &resume);

  // set LED as input and enable interrupts on it
  gpio_enable_input(0, PullDown);
  gpio_enable_interrupt(0, Change);

  while (1) {
    yield_for(&resume);
    resume = 0;
    printf("\tGPIO Interrupt!\n");
  }
}


int main(void) {
  printf("*********************\n");
  printf("GPIO Test Application\n");

  // Set mode to which test you want
  uint8_t mode = 1;

  switch (mode) {
    case 0: gpio_interrupt(); break;
    case 1: gpio_output(); break;
    case 2: gpio_input(); break;
  }

  return 0;
}
