#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <ambient_light.h>
#include <humidity.h>
#include <temperature.h>
#include <timer.h>
#include <button.h>
#include <rng.h>

#include <ieee802154.h>
#include <udp.h>

static unsigned char BUF_BIND_CFG[2 * sizeof(sock_addr_t)];

// counts button presses (any button on board)
static unsigned int btn_cnt;
uint8_t randbuf[4];

void print_ipv6(ipv6_addr_t *);

void print_ipv6(ipv6_addr_t *ipv6_addr) {
  for (int j = 0; j < 14; j += 2) {
    printf("%02x%02x:", ipv6_addr->addr[j], ipv6_addr->addr[j + 1]);
  }
  printf("%02x%02x", ipv6_addr->addr[14], ipv6_addr->addr[15]);
}

// Callback for button presses.
//   btn_num: The index of the button associated with the callback
//   val: 1 if pressed, 0 if depressed
static void button_callback(__attribute__ ((unused)) int btn_num,
                            int val,
                            __attribute__ ((unused)) int arg2,
                            __attribute__ ((unused)) void *ud) {
  if (val == 1) {
    btn_cnt += 1;
  }
}

int main(void) {

  printf("[Buttons] Enabling button callbacks.\n");
  button_subscribe(button_callback, NULL);

  // Enable interrupts on each button.
  int count = button_count();
  for (int i = 0; i < count; i++) {
    button_enable_interrupt(i);
  }

  printf("[UDP] Starting UDP App.\n");

  unsigned int humi = 1;
  int temp = 2;
  int lux  = 3;
  char packet[70];
  btn_cnt = 0;

  ieee802154_set_pan(0xABCD);
  ieee802154_config_commit();
  ieee802154_up();

  ipv6_addr_t ifaces[10];
  udp_list_ifaces(ifaces, 10);

  sock_handle_t handle;
  sock_addr_t addr = {
    ifaces[2],
    15123
  };

  printf("Opening socket on ");
  print_ipv6(&ifaces[2]);
  printf(" : %d\n", addr.port);
  int bind_return = udp_bind(&handle, &addr, BUF_BIND_CFG);

  if (bind_return < 0) {
    printf("Bind failed. Error code: %d\n", bind_return);
    return -1;
  }
// Set the below address to be the IP address of your receiver
  ipv6_addr_t dest_addr = {
    {0xfe, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xfe, 0, 0xbf, 0xf0}
  };
  sock_addr_t destination = {
    dest_addr,
    16123
  };

  printf("[Sensors] All available sensors on the platform will be sampled.\n");
  printf("[RNG] Test App\n");

  while (1) {
    // Some imixes are unable to read sensors due to hardware issues,
    // If this app hangs, comment out the next 3 lines of code

    //temperature_read_sync(&temp);
    //humidity_read_sync(&humi);
    //ambient_light_read_intensity_sync(&lux);

    //get randomness
    int rand_bytes = rng_sync(randbuf, 4, 4);
    if (rand_bytes < 0) {
        printf("Error obtaining random number: %d\n", rand_bytes);
    }
    else if (rand_bytes != 4) {
        printf("Only obtained %d bytes of randomness\n", rand_bytes);
    }

    uint32_t rand = 0;
    memcpy(&rand, randbuf, 4);

    int len = snprintf(packet, sizeof(packet), "rand: %lu; %d deg C; %d%% RH; %d lux; %d btn presses;\n",
                       rand, temp, humi, lux, btn_cnt);
    int max_tx_len = udp_get_max_tx_len();
    if (len > max_tx_len) {
      printf("Cannot send packets longer than %d bytes without changing"
             " constants in kernel\n", max_tx_len);
      return 0;
    }
    printf("Sending packet (length %d) --> ", len);
    print_ipv6(&(destination.addr));
    printf(" : %d\n", destination.port);
    ssize_t result = udp_send_to(packet, len, &destination);

    switch (result) {
      case TOCK_SUCCESS:
        printf("Packet sent.\n\n");
        break;
      default:
        printf("Error sending packet %d\n\n", result);
    }

    delay_ms(5000);
  }
}
