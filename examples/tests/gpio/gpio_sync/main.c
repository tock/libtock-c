#include <stdio.h>

#include <libtock-sync/peripherals/gpio.h>
#include <libtock/interface/console.h>
#include <libtock/tock.h>

int main(void) {
  returncode_t ret;

  printf("[Test] GPIO Sync Interrupt\n");
  printf("Jump GPIO pin 0 low to start test.\n");
  printf("Then move the jumper to VCC\n");

  ret = libtocksync_gpio_wait_until_high(0, libtock_pull_down);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[ERROR] %s\n", tock_strrcode(ret));
    return -1;
  }

  printf("Pin 0 went high\n");
  printf("Now jumper back to ground\n");

  libtocksync_gpio_wait_until_low(0, libtock_pull_up);

  printf("Pin 0 went low\n");
  printf("Now jumper back to high\n");

  libtocksync_gpio_wait_until_changed(0, libtock_pull_down);

  printf("Pin 0 went back high\n");
  printf("Success! Test done\n");

  return 0;
}
