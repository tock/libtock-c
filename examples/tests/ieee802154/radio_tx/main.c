#include <stdbool.h>
#include <stdio.h>

#include <libtock-sync/net/ieee802154.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/interface/led.h>
#include <libtock/net/ieee802154.h>
#include <libtock/peripherals/gpio.h>

// IEEE 802.15.4 sample packet transmission app.
// Continually transmits frames at the specified short address to the specified
// destination address.

#define BUF_SIZE 60
uint8_t packet[BUF_SIZE];
bool toggle = true;

int main(void) {
  int i;
  for (i = 0; i < BUF_SIZE; i++) {
    packet[i] = i;
  }
  libtock_gpio_enable_output(0);
  libtock_ieee802154_set_address_short(0x1540);
  libtock_ieee802154_set_pan(0xABCD);
  libtock_ieee802154_config_commit();
  libtocksync_ieee802154_up();
  while (1) {
    libtock_led_toggle(0);
    int err = libtocksync_ieee802154_send(0x0802,
                                          SEC_LEVEL_NONE,
                                          0,
                                          NULL,
                                          packet,
                                          BUF_SIZE);
    if (err == RETURNCODE_SUCCESS) {
      printf("Transmitted successfully.\n");
    } else if (err == RETURNCODE_ENOACK) {
      printf("Transmitted but packet not acknowledged.\n");
      libtock_gpio_toggle(0);
    } else {
      printf("Transmit failed with error %i.\n", err);
      libtock_gpio_toggle(0);
    }
    libtocksync_alarm_delay_ms(250);
  }
}
