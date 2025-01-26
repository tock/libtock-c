#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/interface/button.h>
#include <libtock/interface/led.h>
#include <libtock/net/ieee802154.h>
#include <libtock/net/udp.h>
#include <libtock/peripherals/rng.h>
#include <libtock/sensors/ambient_light.h>
#include <libtock/sensors/humidity.h>
#include <libtock/sensors/temperature.h>

#include <libtock-sync/net/udp.h>
#include <libtock/net/udp.h>

#define DEBUG 1

static unsigned char BUF_BIND_CFG[2 * sizeof(sock_addr_t)];

void print_ipv6(ipv6_addr_t*);
int serialize_to_json(char* packet, int len, uint32_t rand, int temp, int humi, int lux);

int main(void) {

  printf("[UDP] Starting UDP_Send Test App.\n");

  static char packet[70];

  ipv6_addr_t ifaces[10];
  libtock_udp_list_ifaces(ifaces, 10);

  sock_handle_t handle;
  sock_addr_t addr = {
    ifaces[0],
    11111
  };

  printf("Opening socket on ");
  print_ipv6(&ifaces[0]);
  printf(" : %d\n", addr.port);
  int bind_return = libtock_udp_bind(&handle, &addr, BUF_BIND_CFG);

  if (bind_return < 0) {
    printf("Bind failed. Error code: %d\n", bind_return);
    return -1;
  }

  // Set the below address to be the IP address of your receiver
  // The current address corresponds to the default src address
  // set in the corresponding udp_rx app
  ipv6_addr_t dest_addr = {
    {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
      0x1c, 0x1d, 0x1e, 0x1f}
  };
  sock_addr_t destination = {
    dest_addr,
    16123
  };

  int count = 0;

  while (1) {
    int len = snprintf(packet, sizeof(packet), "Hello World (Packet #: %d)\n",
                       count);
    if (DEBUG) {
      printf("Sending packet (length %d) --> ", len);
      print_ipv6(&(destination.addr));
      printf(" : %d\n", destination.port);
    }
    returncode_t result = libtocksync_udp_send(packet, len, &destination);

    switch (result) {
      case RETURNCODE_SUCCESS:
        if (DEBUG) {
          printf("Packet sent.\n\n");
        }
        break;
      default:
        printf("Error sending packet %d\n\n", result);
    }
    count++;
    libtocksync_alarm_delay_ms(5000);
  }
}

void print_ipv6(ipv6_addr_t* ipv6_addr) {
  for (int j = 0; j < 14; j += 2) {
    printf("%02x%02x:", ipv6_addr->addr[j], ipv6_addr->addr[j + 1]);
  }
  printf("%02x%02x", ipv6_addr->addr[14], ipv6_addr->addr[15]);
}
