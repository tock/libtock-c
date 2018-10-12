#include <stdbool.h>
#include <stdio.h>

#include <button.h>
#include <ambient_light.h>
#include <timer.h>

#include <ieee802154.h>
#include <udp.h>

void print_ipv6(ipv6_addr_t *);

void print_ipv6(ipv6_addr_t *ipv6_addr) {
  for (int j = 0; j < 14; j += 2) {
    printf("%02x%02x:", ipv6_addr->addr[j], ipv6_addr->addr[j + 1]);
  }
  printf("%02x%02x", ipv6_addr->addr[14], ipv6_addr->addr[15]);
}

int main(void) {

  int lux  = 1;
  char packet[64];

  ieee802154_set_pan(0xABCD);
  ieee802154_config_commit();
  ieee802154_up();

  ipv6_addr_t ifaces[10];
  udp_list_ifaces(ifaces, 10);

  sock_handle_t handle;
  sock_addr_t addr = {
    ifaces[0],
    15123
  };

  printf("Opening socket on ");
  print_ipv6(&ifaces[0]);
  printf(" : %d\n", addr.port);
  udp_socket(&handle, &addr);

  sock_addr_t destination = {
    ifaces[1],
    16123
  };

  while (1) {
    //wait for gpio pin to be pressed
    int button_val = 0;
    while(button_val == 0) {
        delay_ms(1);
        button_val = button_read(0);
    }

    ambient_light_read_intensity_sync(&lux);
    int len = snprintf(packet, sizeof(packet), "%d lux;\n", lux);

    printf("Sending packet (length %d) --> ", len);
    print_ipv6(&(destination.addr));
    printf(" : %d\n", destination.port);
    ssize_t result = udp_send_to(&handle, packet, len, &destination);

    switch (result) {
      case TOCK_SUCCESS:
        printf("Packet sent.\n");
        break;
      case TOCK_ENOACK:
        printf("Sent but not acknowledged\n");
        break;
      default:
        printf("Error sending packet %d\n", result);
    }

    while(button_val == 1) {
        delay_ms(1);
        button_val = button_read(0);
    }
  }

  udp_close(&handle);
}
