#include <stdbool.h>
#include <stdio.h>

#include <libtock-sync/net/ieee802154.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/interface/led.h>
#include <libtock/net/ieee802154.h>

// IEEE 802.15.4 sample packet echo app.
// Continually receives packets at the specified short address, then retransmits them.
// Use this in conjunction with the radio_tx app.

libtock_ieee802154_rxbuf rx_ring_buf;
bool toggle = true;

int main(void) {
  unsigned short ADDR = 0x0802;
  unsigned short PAN  = 0xABCD;
  libtock_ieee802154_set_address_short(ADDR);
  libtock_ieee802154_set_pan(PAN);
  libtock_ieee802154_config_commit();
  libtocksync_ieee802154_up();
  while (1) {
    if (libtocksync_ieee802154_receive(&rx_ring_buf) >= 0) {
      unsigned short pan;
      unsigned short short_addr;

      uint8_t* packet_rx = libtock_ieee802154_read_next_frame(&rx_ring_buf);
      while (packet_rx) {
        if (!libtock_ieee802154_frame_get_dst_pan(packet_rx, &pan) || pan != PAN) {
          continue;
        }

        addr_mode_t mode = libtock_ieee802154_frame_get_dst_addr(packet_rx, &short_addr, NULL);
        if (mode != ADDR_SHORT || short_addr != ADDR) {
          continue;
        }

        // Only retransmit the payload if this packet was meant for us.
        int payload_offset = libtock_ieee802154_frame_get_payload_offset(packet_rx);
        int payload_length = libtock_ieee802154_frame_get_payload_length(packet_rx);
        libtocksync_alarm_delay_ms(250);
        printf("Retransmitting received packet.\n");
        libtocksync_ieee802154_send(0xFFFF,
                                    SEC_LEVEL_NONE,
                                    0,
                                    NULL,
                                    packet_rx + payload_offset,
                                    payload_length);
        packet_rx = libtock_ieee802154_read_next_frame(&rx_ring_buf);
      }
      libtock_led_toggle(0);
    }
  }
}
