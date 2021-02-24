#include "math.h"
#include <stdbool.h>
#include <stdio.h>

#include <ble_advdata.h>
#include <nordic_common.h>
#include <nrf_error.h>

#include <simple_adv.h>
#include <simple_ble.h>

#include <adc.h>
#include <ambient_light.h>
#include <button.h>
#include <gpio.h>
#include <humidity.h>
#include <led.h>
#include <ninedof.h>
#include <nrf51_serialization.h>
#include <temperature.h>
#include <timer.h>
#include <udp.h>

// Intervals for BLE advertising and connections
simple_ble_config_t ble_config = {
  .platform_id       = 0x13,                // used as 4th octect in device BLE address
  .device_id         = DEVICE_ID_DEFAULT,
  .adv_name          = (char*)"imix",
  .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
  .min_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
  .max_conn_interval = MSEC_TO_UNITS(1250, UNIT_1_25_MS),
};

// Empty handler for setting BLE addresses
void ble_address_set (void) {
  // nop
}

// Buffer for UDP Bindings
static unsigned char BUF_BIND_CFG[2 * sizeof(sock_addr_t)];

// Callback for button presses.
//   btn_num: The index of the button associated with the callback
//   val: 1 if pressed, 0 if depressed
static void button_callback(__attribute__ ((unused)) int btn_num,
                            int val,
                            __attribute__ ((unused)) int arg2,
                            __attribute__ ((unused)) void *ud) {
  if (val == 1) {
    led_on(0);
  } else {
    led_off(0);
  }
}

// struct for ninedof results
struct ninedof_data {
  int x;
  int y;
  int z;
  bool fired;
};
// callback for ninedof
static void ninedof_cb(int x, int y, int z, void* ud) {
  struct ninedof_data* result = (struct ninedof_data*) ud;
  result->x     = x;
  result->y     = y;
  result->z     = z;
  result->fired = true;
}

static void sample_sensors (void) {

  // Sensors: temperature/humidity, acceleration, light
  int temp;
  temperature_read_sync(&temp);
  unsigned humi;
  humidity_read_sync(&humi);

  /* some Imix boards do not actually have accelerometer, so use async in such
   * a way that its absence does not hang the app */
  uint32_t accel_mag = 0;

  struct ninedof_data result = { .fired = false };

  ninedof_subscribe(ninedof_cb, (void*)(&result));
  ninedof_start_accel_reading();
  delay_ms(5); // let ninedof cb execute if accelerometer is on board

  if (&result.fired) {
    accel_mag = sqrt(result.x * result.x + result.y * result.y + result.z * result.z);
  }

  int light;
  ambient_light_read_intensity_sync(&light);

  // Analog inputs: A0-A5
  uint16_t val;
  adc_sample_sync(0, &val);
  int a0 = (val * 3300) / (4095 << 4);
  adc_sample_sync(1, &val);
  int a1 = (val * 3300) / (4095 << 4);
  adc_sample_sync(2, &val);
  int a2 = (val * 3300) / (4095 << 4);
  adc_sample_sync(3, &val);
  int a3 = (val * 3300) / (4095 << 4);
  adc_sample_sync(4, &val);
  int a4 = (val * 3300) / (4095 << 4);
  adc_sample_sync(5, &val);
  int a5 = (val * 3300) / (4095 << 4);

  // Digital inputs: D0, D1, D6, D7
  int d0 = gpio_read(0);
  int d1 = gpio_read(1);
  int d6 = gpio_read(2);
  int d7 = gpio_read(3);

  // print results
  printf("[imix Sensor Reading]\n");
  printf("  Temperature:  %d 1/100 degrees C\n", temp);
  printf("  Humidity:     %u 0.01%%\n", humi);
  printf("  Light:        %d\n", light);
  printf("  Acceleration: %lu\n", accel_mag);
  printf("  A0:           %d mV\n", a0);
  printf("  A1:           %d mV\n", a1);
  printf("  A2:           %d mV\n", a2);
  printf("  A3:           %d mV\n", a3);
  printf("  A4:           %d mV\n", a4);
  printf("  A5:           %d mV\n", a5);
  printf("  D0:           %d\n", d0);
  printf("  D1:           %d\n", d1);
  printf("  D6:           %d\n", d6);
  printf("  D7:           %d\n", d7);
  printf("\n");

  // toggle the user LED
  led_toggle(1);
}

static void send_udp_packet(void) {
  char packet[64];
  ipv6_addr_t dest_addr = {
    {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
     0x1c, 0x1d, 0x1e, 0x1f}
  };
  sock_addr_t destination = {
    dest_addr,
    16123
  };

  int len        = snprintf(packet, sizeof(packet), "Tock Running on Imix!\n");
  ssize_t result = udp_send_to(packet, len, &destination);
  if (result != TOCK_SUCCESS) {
    printf("Error sending packet %d\n\n", result);
  } else {
    printf("UDP Packet sent successfully\n");
  }
}

int main(void) {
  printf("[imix] Test App!\n");
  printf("[imix] Samples all sensors.\n");
  printf("[imix] Transmits name over BLE.\n");
  printf("[imix] Button controls LED.\n");

  // Setup BLE
  simple_ble_init(&ble_config);
  simple_adv_only_name();

  // Enable button callbacks
  button_subscribe(button_callback, NULL);
  button_enable_interrupt(0);

  // Setup D0, D1, D6, D7
  gpio_enable_input(0, PullDown); // D0
  gpio_enable_input(1, PullDown); // D1
  gpio_enable_input(2, PullDown); // D6
  gpio_enable_input(3, PullDown); // D7

  // setup UDP/IP
  ipv6_addr_t ifaces[10];
  udp_list_ifaces(ifaces, 10);

  sock_handle_t handle;
  sock_addr_t addr = {
    ifaces[0],
    11111
  };

  int bind_return = udp_bind(&handle, &addr, BUF_BIND_CFG);
  if (bind_return < 0) {
    printf("Bind failed. Error code: %d\n", bind_return);
  }

  // sample sensors every second
  while (1) {
    sample_sensors();
    send_udp_packet();
    delay_ms(1000);
  }
}
