#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <libtock-sync/net/ieee802154.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/interface/led.h>
#include <libtock/net/ieee802154.h>
#include <libtock/peripherals/gpio.h>

// IEEE 802.15.4 sample packet transmission app.
// Continually transmits 15.4 broadcast frames.

#define BUF_SIZE 75
#define PAYLOAD_SIZE 60
#define PAYLOAD_OFFSET 15

// IEEE 802.15.4 frame buffer
uint8_t buf[BUF_SIZE];

// IEEE 802.15.4 frame control fields
// - Frame type: Data frame (0x1)
// - Security enabled: No (0x0)
// - Frame pending: No (0x0)
// - Acknowledgement request: No (0x0)
// - PAN ID compression: True (0x1)
// - Reserved: (0x0)
// - Sequence Number Suppression: False (0x0)
// - Destination Addressing Mode: Short/16-bit (0x2)
// - Frame Version: IEEE Std 802.15.4-2006 (0x1)
// - Source Addressing Mode: Long/64-bit (0x3)
uint8_t frame_control_fields[2] = {0x41, 0xd8};

// IEEE 802.15.4 frame sequence number.
uint8_t sequence_number = 0x00;
#define SEQUENCE_NUMBER_OFFSET 2

// IEEE 802.15.4 frame pan/addressing fields.
uint8_t destination_pan[2]     = {0xcd, 0xab};
uint8_t destination_address[2] = {0xff, 0xff};


static void form_packet(void) {
  // Copy frame control fields.
  memcpy(buf, frame_control_fields, 2);

  // Copy sequence number.
  buf[SEQUENCE_NUMBER_OFFSET] = sequence_number;

  // Copy destination PAN.
  memcpy(buf + 3, destination_pan, 2);

  // Copy destination address.
  memcpy(buf + 5, destination_address, 2);

  // Get the extended address of the device.
  uint8_t addr[8];
  libtock_ieee802154_get_address_long(addr);

  // Copy the extended source address.
  memcpy(buf + 7, addr, 8);

  // Copy payload.
  for (int i = 0; i < PAYLOAD_SIZE; i++) {
    buf[PAYLOAD_OFFSET + i] = i;
  }
}

bool toggle = true;

int main(void) {
  // Initialize and form the frame buffer.
  form_packet();

  // Enable the GPIO pin for toggling.
  libtock_gpio_enable_output(0);

  // Set the PAN ID and commit the configuration.
  libtock_ieee802154_set_pan(0xABCD);
  libtock_ieee802154_config_commit();

  // Enable the radio.
  libtocksync_ieee802154_up();
  while (1) {
    libtock_led_toggle(0);
    int err = libtocksync_ieee802154_send_raw(buf,
                                              BUF_SIZE);

    sequence_number++;
    if (err == RETURNCODE_SUCCESS) {
      printf("Transmitted successfully.\n");
    } else if (err == RETURNCODE_ENOACK) {
      printf("Transmitted but packet not acknowledged.\n");
      libtock_gpio_toggle(0);
    } else {
      printf("Transmit failed with error %i.\n", err);
      libtock_gpio_toggle(0);
    }
    libtocksync_alarm_delay_ms(500);
  }
}
