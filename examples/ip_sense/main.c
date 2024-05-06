#include <stdbool.h>
#include <stdio.h>

#include <libtock-sync/sensors/ambient_light.h>
#include <libtock-sync/sensors/humidity.h>
#include <libtock-sync/sensors/temperature.h>
#include <libtock-sync/services/alarm.h>

#include <libtock-sync/net/ieee802154.h>
#include <libtock-sync/net/udp.h>

static unsigned char BUF_BIND_CFG[2 * sizeof(sock_addr_t)];

void print_ipv6(ipv6_addr_t *);

void print_ipv6(ipv6_addr_t *ipv6_addr) {
  for (int j = 0; j < 14; j += 2) {
    printf("%02x%02x:", ipv6_addr->addr[j], ipv6_addr->addr[j + 1]);
  }
  printf("%02x%02x", ipv6_addr->addr[14], ipv6_addr->addr[15]);
}

int main(void) {
  printf("[IPv6_Sense] Starting IPv6 Sensors App.\n");
  printf("[IPv6_Sense] Sensors will be sampled and transmitted.\n");

  int humi = 0;
  int temp = 0;
  int lux  = 0;
  char packet[64];

  libtock_ieee802154_set_pan(0xABCD);
  libtock_ieee802154_config_commit();
  libtocksync_ieee802154_up();

  ipv6_addr_t ifaces[10];
  libtock_udp_list_ifaces(ifaces, 10);

  sock_handle_t handle;
  sock_addr_t addr = {
    ifaces[0],
    15123
  };

  printf("Opening socket on ");
  print_ipv6(&ifaces[0]);
  printf(" : %d, and binding to that socket.\n", addr.port);
  int bind_return = libtock_udp_bind(&handle, &addr, BUF_BIND_CFG);

  if (bind_return < 0) {
    printf("Failed to bind to port: failure=%d\n", bind_return);
    return -1;
  }

  sock_addr_t destination = {
    ifaces[1],
    16123
  };

  while (1) {
    libtocksync_temperature_read(&temp);
    libtocksync_humidity_read(&humi);
    libtocksync_ambient_light_read_intensity(&lux);

    int len = snprintf(packet, sizeof(packet), "%d deg C; %d%% humidity; %d lux;\n",
                       temp / 100, humi / 100, lux);
    int max_tx_len;
    libtock_udp_get_max_tx_len(&max_tx_len);
    if (len > max_tx_len) {
      printf("Cannot send packets longer than %d bytes without changing"
             " constants in kernel\n", max_tx_len);
      return 0;
    }
    printf("Sending packet (length %d) --> ", len);
    print_ipv6(&(destination.addr));
    printf(" : %d\n", destination.port);
    ssize_t result = libtocksync_udp_send(packet, len, &destination);

    switch (result) {
      case RETURNCODE_SUCCESS:
        printf("Packet sent.\n\n");
        break;
      default:
        printf("Error sending packet %d\n\n", result);
    }
    libtocksync_alarm_delay_ms(4000);
  }
}
