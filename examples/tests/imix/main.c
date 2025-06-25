#include <ble_advdata.h>
#include <math.h>
#include <nordic_common.h>
#include <nrf_error.h>
#include <simple_adv.h>
#include <simple_ble.h>
#include <stdbool.h>
#include <stdio.h>

#include <libtock-sync/peripherals/adc.h>
#include <libtock-sync/peripherals/crc.h>
#include <libtock-sync/peripherals/rng.h>
#include <libtock-sync/sensors/ambient_light.h>
#include <libtock-sync/sensors/humidity.h>
#include <libtock-sync/sensors/ninedof.h>
#include <libtock-sync/sensors/temperature.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/interface/button.h>
#include <libtock/interface/led.h>
#include <libtock/net/nrf51_serialization.h>
#include <libtock/peripherals/gpio.h>

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
void ble_address_set(void) {
  // nop
}

// Callback for button presses.
//   btn_num: The index of the button associated with the callback
//   val: 1 if pressed, 0 if depressed
static void button_callback(
  __attribute__ ((unused)) returncode_t ret,
  __attribute__ ((unused)) int          btn_num,
  bool                                  val) {
  if (val) {
    libtock_led_on(0);
  } else {
    libtock_led_off(0);
  }
}

static void sample_sensors(void) {
  printf("[imix Sensor Reading]\n");
  // Sensors: temperature/humidity, acceleration, light
  int temp;
  libtocksync_temperature_read(&temp);
  printf("  Temperature:  %d.%02d degrees C\n", temp / 100, temp % 100);

  int humi;
  libtocksync_humidity_read(&humi);
  printf("  Humidity:     %u.%02u%%\n", humi / 100, humi % 100);

  double accel_mag;
  libtocksync_ninedof_read_accelerometer_magnitude(&accel_mag);
  printf("  Acceleration: %f\n", accel_mag);

  int light;
  libtocksync_ambient_light_read_intensity(&light);
  printf("  Light:        %d\n", light);

  // Analog inputs: A0-A5
  for (int a = 0; a < 6; a++) {
    uint16_t val;
    libtocksync_adc_sample(a, &val);
    int ival = (val * 3300) / (4095 << 4);
    printf("  A%i:           %d mV\n", a, ival);
  }

  // Digital inputs: D0, D1, D6, D7
  for (int d = 0; d < 4; d++) {
    int val;
    libtock_gpio_read(0, &val);
    printf("  D%i:           %d\n", d, val);
  }
  printf("\n");

  // toggle the user LED
  libtock_led_toggle(1);
}

int main(void) {
  printf("[imix] Test App!\n");
  printf("[imix] Samples all sensors.\n");
  printf("[imix] Transmits name over BLE.\n");
  printf("[imix] Button controls LED.\n");

  // Setup BLE
  simple_ble_init(&ble_config);
  printf("[imix] BLE initialized.\n");
  simple_adv_only_name();
  printf("[imix] BLE advertising.\n");

  // Enable button callbacks
  libtock_button_notify_on_press(0, button_callback);
  printf("[imix] Button initialized.\n");

  // Setup D0, D1, D6, D7
  libtock_gpio_enable_input(0, libtock_pull_down); // D0
  libtock_gpio_enable_input(1, libtock_pull_down); // D1
  libtock_gpio_enable_input(2, libtock_pull_down); // D6
  libtock_gpio_enable_input(3, libtock_pull_down); // D7
  printf("[imix] GPIO D0, D1, D6, D7 configured to be pulldown.\n");

  // Should add UDP here

  // sample sensors every second
  while (1) {
    printf("[imix] Sampling sensors.\n");
    sample_sensors();
    libtocksync_alarm_delay_ms(1000);
  }
}
