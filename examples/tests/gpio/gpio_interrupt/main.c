#include <stdio.h>

#include <libtock/interface/console.h>
#include <libtock/peripherals/gpio.h>
#include <libtock/tock.h>

static void gpio_cb (__attribute__ ((unused)) uint32_t pin_num,
                     __attribute__ ((unused)) bool     val) {}

int main(void) {
  printf("[Test] GPIO Interrupt\n");
  printf("Jump GPIO pin 0 high to test.\n");

  // set callback for GPIO interrupts
  libtock_gpio_set_interrupt_callback(gpio_cb);

  // set GPIO 0 as input and enable interrupts on it
  libtock_gpio_enable_input(0, libtock_pull_down);
  libtock_gpio_enable_interrupt(0, libtock_change);

  while (1) {
    yield();
    printf("\tGPIO Interrupt! (App: %p)\n", tock_app_flash_begins_at());
  }

  return 0;
}
