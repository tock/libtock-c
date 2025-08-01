#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/interface/led.h>
#include <libtock/net/udp.h>

/*
 * UDP sample packet reception app.
 * Receives packets at the specified address and port for 30 seconds,
 * then closes the socket.
 * Each time a packet is received, the user LED blinks
 */

#define MAX_RX_PACKET_LEN 200

char packet_rx[MAX_RX_PACKET_LEN];
static unsigned char BUF_BIND_CFG[2 * sizeof(sock_addr_t)];
sock_handle_t* handle;

void print_ipv6(ipv6_addr_t*);

void print_ipv6(ipv6_addr_t* ipv6_addr) {
  for (int j = 0; j < 14; j += 2) {
    printf("%02x%02x:", ipv6_addr->addr[j], ipv6_addr->addr[j + 1]);
  }
  printf("%02x%02x", ipv6_addr->addr[14], ipv6_addr->addr[15]);
}

static void callback(returncode_t ret,
                     int          payload_len) {

  if (ret != RETURNCODE_SUCCESS) {
    printf("Error in receiving packet: %d\n", ret);
    return;
  }

  libtock_led_toggle(0);

#define PRINT_STRING 1
#if PRINT_STRING
  printf("[UDP_RCV_APP1]: Rcvd UDP Packet from: ");
  print_ipv6((ipv6_addr_t*)&BUF_BIND_CFG);
  printf(" : %d\n", (uint16_t)(BUF_BIND_CFG[16]) + ((uint16_t)(BUF_BIND_CFG[17]) << 8));
  printf("Packet Payload: %.*s\n", payload_len, packet_rx);
#else
  for (i = 0; i < payload_len; i++) {
    printf("%02x%c", packet_rx[i],
           ((i + 1) % 16 == 0 || i + 1 == payload_len) ? '\n' : ' ');
  }
#endif // PRINT_STRING
}

int main(void) {

  ipv6_addr_t ifaces[10];
  libtock_udp_list_ifaces(ifaces, 10);

  sock_addr_t addr = {
    ifaces[1],
    16123
  };

  printf("Opening socket on ");
  print_ipv6(&ifaces[1]);
  printf(" : %d, and binding to that socket.\n", addr.port);
  sock_handle_t h;
  handle = &h;
  libtock_udp_bind(handle, &addr, BUF_BIND_CFG);

  memset(packet_rx, 0, MAX_RX_PACKET_LEN);
  returncode_t result = libtock_udp_recv(packet_rx, MAX_RX_PACKET_LEN, callback);

  switch (result) {
    case RETURNCODE_SUCCESS:
      printf("Succesfully bound to socket, listening for UDP packets\n\n");
      break;
    case RETURNCODE_EINVAL:
      printf("The address requested is not a local interface\n");
      break;
    case RETURNCODE_EBUSY:
      printf("Another userland app has already bound to this addr/port\n");
      break;
    case RETURNCODE_ERESERVE:
      printf("Receive Failure. Must bind to a port before calling receive\n");
      break;
    default:
      printf("Failed to bind to socket %d\n", result);
      break;
  }
  /* This app tests receiving for 10 seconds
   * then closing the connection, so we include a delay for that
   * reason. */
  libtocksync_alarm_delay_ms(30000);
  result = libtock_udp_close(handle);
  if (result != RETURNCODE_SUCCESS) {
    printf("Error closing socket\n");
  } else {
    printf("Socket closed.\n");
  }
}
