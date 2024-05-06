#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <libtock-sync/net/ieee802154.h>
#include <libtock/interface/led.h>
#include <libtock/net/ieee802154.h>
#include <libtock-sync/services/alarm.h>

// IEEE 802.15.4 sample packet reception app.
// Continually receives frames at the specified short address. Setting the
// source short address does not cause the radio to filter incoming frames, but
// does cause it to respond to the transmitter with ACKs. Toggles LED whenever
// a frame is received.
//
// Optionally uses the packet inspection functions to print out relevant
// information from the frame. (Set PRINT_PAYLOAD to 1)

libtock_ieee802154_rxbuf rx_ring_buf;

static void callback(__attribute__ ((unused)) int pans,
                     __attribute__ ((unused)) int dst_addr,
                     __attribute__ ((unused)) int src_addr) {
  libtock_led_toggle(0);

  // Before accessing an "allowed" buffer, we must request it back.
  // We do this with the reset_ring_buf function. Because this example
  // only uses one ring buffer, we pass null values to unallow the ringbuffer.
  libtock_reset_ring_buf(NULL, NULL, NULL);

  uint8_t* packet_rx = libtock_ieee802154_read_next_frame(&rx_ring_buf);

  #define PRINT_PAYLOAD 1
  #define PRINT_STRING 0
  while (packet_rx) {
    #if PRINT_PAYLOAD
    int payload_offset = libtock_ieee802154_frame_get_payload_offset(packet_rx);
    int payload_length = libtock_ieee802154_frame_get_payload_length(packet_rx);
    printf("Received packet with payload of %d bytes from offset %d\n", payload_length, payload_offset);
    #if PRINT_STRING
    printf("%.*s\n", payload_length, packet_rx + payload_offset);
    #else
    int j;
    for (j = 0; j < payload_length; j++) {
      printf("%02x%c", packet_rx[payload_offset + j],
             ((j + 1) % 16 == 0 || j + 1 == payload_length) ? '\n' : ' ');
    }
    #endif // PRINT_STRING

    unsigned short pan;
    unsigned short short_addr;
    unsigned char long_addr[8];
    addr_mode_t mode;

    if (libtock_ieee802154_frame_get_dst_pan(packet_rx, &pan)) {
      printf("Packet destination PAN ID: 0x%04x\n", pan);
    } else {
      printf("Packet destination PAN ID: not present\n");
    }

    mode = libtock_ieee802154_frame_get_dst_addr(packet_rx, &short_addr, long_addr);
    if (mode == ADDR_NONE) {
      printf("Packet destination address: not present\n");
    } else if (mode == ADDR_SHORT) {
      printf("Packet destination address: 0x%04x\n", short_addr);
    } else if (mode == ADDR_LONG) {
      printf("Packet destination address:");
      for (int i = 0; i < 8; i++) {
        printf(" %02x", long_addr[i]);
      }
      printf("\n");
    }

    if (libtock_ieee802154_frame_get_src_pan(packet_rx, &pan)) {
      printf("Packet source PAN ID: 0x%04x\n", pan);
    } else {
      printf("Packet source PAN ID: not present\n");
    }

    mode = libtock_ieee802154_frame_get_src_addr(packet_rx, &short_addr, long_addr);
    if (mode == ADDR_NONE) {
      printf("Packet source address: not present\n");
    } else if (mode == ADDR_SHORT) {
      printf("Packet source address: 0x%04x\n", short_addr);
    } else if (mode == ADDR_LONG) {
      printf("Packet source address:");
      for (int i = 0; i < 8; i++) {
        printf(" %02x", long_addr[i]);
      }
      printf("\n");
    }
    #endif

    packet_rx = libtock_ieee802154_read_next_frame(&rx_ring_buf);
  }

  libtock_ieee802154_receive(&rx_ring_buf, callback);
}

int main(void) {
  memset(rx_ring_buf, 0, libtock_ieee802154_RING_BUFFER_LEN);
  libtock_ieee802154_set_address_short(0x802);
  libtock_ieee802154_set_pan(0xABCD);
  libtock_ieee802154_config_commit();
  libtocksync_ieee802154_up();
  libtock_ieee802154_receive(&rx_ring_buf, callback);
  while (1) {
    libtocksync_alarm_delay_ms(4000);
  }
}
