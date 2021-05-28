#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <console.h>
#include <gpio.h>
#include <led.h>
#include <timer.h>
#include <tock.h>

/* The gpio pin gets toggled every 1000 ms */
#define TOGGLE_PERIOD 1000

/* The number of toggle for each pin */
#define TOGGLE_COUNT 4

/* Amount of cycle until timeout is claimed */
#define TIMEOUT 1

/* Maximum GPIO pin number */
#define MAX_PIN_NO 15

/* Prototype */
uint8_t test_pin(bool *);
void set_gpios(void);


int callback_count = 0;
// callback for timers
static void timer_cb (__attribute__ ((unused)) int arg0,
                      __attribute__ ((unused)) int arg1,
                      __attribute__ ((unused)) int arg2,
                      __attribute__ ((unused)) void* userdata) {
  callback_count     = callback_count + 1;
  *((bool*)userdata) = 1;
}

void set_gpios(void) {
  GPIO_Pin_t i = 0;

  for (; i <= MAX_PIN_NO; i++) {
    gpio_enable_output(i);
  }

  /* Set pin to high */
  for (i = 0; i <= MAX_PIN_NO; i++) {
    gpio_set(i);
  }
}

uint8_t test_pin(bool * int_flag) {
  uint8_t attempt_count = 0;

  while (attempt_count < TIMEOUT) {    

    for (uint8_t i = 0; i < TOGGLE_COUNT; i++) {
      yield_for(int_flag);
      *int_flag = 0; /* Reset indicator */
      for (GPIO_Pin_t j = 0; j <= MAX_PIN_NO; j++) {
        gpio_toggle(j);
      }
    }

    attempt_count++;
  }

  return 1;
}

static void gpio_test(void) {
  tock_timer_t timer; /* Timer object */
  static bool int_flag = 0; /* Toggle trigger */

  /* Initiate repeating timer */
  timer_every(TOGGLE_PERIOD, timer_cb, &int_flag, &timer);

  set_gpios();

  /* Main loop */
  while (1) {
    test_pin(&int_flag);
  }
}

int main(void) {
  gpio_test();
  return 0;
}
