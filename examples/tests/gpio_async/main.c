#include <stdio.h>

#include <libtock-sync/peripherals/gpio_async.h>
#include <libtock-sync/services/alarm.h>

int interrupt_count = 0;

static void gpio_async_cb(uint32_t port, uint32_t pin, bool value) {
  printf("INTERRUPT port:%li pin:%li val:%i\n", port, pin, value);
  interrupt_count++;

  if (interrupt_count > 5) {
    printf("Now disabling interrupt\n");
    libtocksync_gpio_async_disable_interrupt(0, 1);
  }
}

int main (void) {
  printf("[TEST] GPIO Async\n");

  printf("Enabling rising edge interrupt on port 0 pin 1\n");
  libtock_gpio_async_set_interrupt_callback(gpio_async_cb);
  libtocksync_gpio_async_make_input(0, 1, libtock_pull_none);
  libtocksync_gpio_async_enable_interrupt(0, 1, libtock_rising_edge);

  printf("Toggling port 0 pin 0\n");
  libtocksync_gpio_async_make_output(0, 0);

  while (1) {
    libtocksync_gpio_async_set(0, 0);
    libtocksync_alarm_delay_ms(500);
    libtocksync_gpio_async_clear(0, 0);
    libtocksync_alarm_delay_ms(500);
  }

  return 0;
}
