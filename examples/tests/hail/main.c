#include <stdbool.h>
#include <stdio.h>

#include <ble_advdata.h>
#include <nordic_common.h>
#include <nrf_error.h>

#include <simple_adv.h>
#include <simple_ble.h>

#include <libtock-sync/peripherals/crc.h>
#include <libtock-sync/peripherals/rng.h>
#include <libtock-sync/sensors/ambient_light.h>
#include <libtock-sync/sensors/humidity.h>
#include <libtock-sync/sensors/ninedof.h>
#include <libtock-sync/sensors/temperature.h>
#include <libtock/interface/button.h>
#include <libtock/interface/led.h>
#include <libtock/net/nrf51_serialization.h>
#include <libtock/peripherals/adc.h>
#include <libtock/peripherals/gpio.h>
#include <libtock/timer.h>


/////////////////////////////////////////////////////////////////////
// Software CRC implementation for validating CRC driver
//
// From http://home.thep.lu.se/~bjorn/crc/ (April 22, 2020)
static uint32_t crc32_for_byte(uint32_t r) {
  for (int j = 0; j < 8; ++j)
    r = (r & 1 ? 0 : (uint32_t)0xEDB88320L) ^ r >> 1;
  return r ^ (uint32_t)0xFF000000L;
}

static void reference_crc32(const void *data, size_t n_bytes, uint32_t* crc) {
  static uint32_t table[0x100];
  if (!*table)
    for (size_t i = 0; i < 0x100; ++i)
      table[i] = crc32_for_byte(i);
  for (size_t i = 0; i < n_bytes; ++i)
    *crc = table[(uint8_t)*crc ^ ((uint8_t*)data)[i]] ^ *crc >> 8;
}
/////////////////////////////////////////////////////////////////////



// Intervals for BLE advertising and connections
simple_ble_config_t ble_config = {
  .platform_id       = 0x13,                // used as 4th octect in device BLE address
  .device_id         = DEVICE_ID_DEFAULT,
  .adv_name          = (char*)"Hail",
  .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
  .min_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
  .max_conn_interval = MSEC_TO_UNITS(1250, UNIT_1_25_MS),
};

// Empty handler for setting BLE addresses
void ble_address_set (void) {
  // nop
}

// Callback for button presses.
//   btn_num: The index of the button associated with the callback
//   val: 1 if pressed, 0 if depressed
static void button_callback(__attribute__ ((unused)) returncode_t ret,
                            __attribute__ ((unused)) int          btn_num,
                            bool                                  val) {
  if (val == 1) {
    libtock_led_on(1); // green
  } else {
    libtock_led_off(1);
  }
}

// Callback for gpio interrupts.
//   - pin_num: The index of the pin associated with the callback.
//   - pin_state: 1 if high, 0 if low.
static void gpio_callback(  uint32_t pin_num,
                            bool     pin_state) {
  printf("GPIO Interrupt: pin: %li, state: %i\n", pin_num, pin_state);
}

static void sample_sensors (void) {

  // Sensors: temperature/humidity, acceleration, light
  int temp;
  libtocksync_temperature_read(&temp);
  int humi;
  libtocksync_humidity_read(&humi);
  double accel_mag;
  libtocksync_ninedof_read_accelerometer_magnitude(&accel_mag);
  int light;
  libtocksync_ambient_light_read_intensity(&light);

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
  int d0;
  libtock_gpio_read(0, &d0);
  int d1;
  libtock_gpio_read(1, &d1);
  int d6;
  libtock_gpio_read(2, &d6);
  int d7;
  libtock_gpio_read(3, &d7);

  // Random bytes
  uint8_t rand[5];
  int count;
  libtocksync_rng_get_random_bytes(rand, 5, 5, &count);

  // CRC of the random bytes
  uint32_t crc;
  libtocksync_crc_compute(rand, 5, LIBTOCK_CRC_32, &crc);
  uint32_t reference_crc = 0;
  reference_crc32(rand, 5, &reference_crc);

  // print results
  printf("[Hail Sensor Reading]\n");
  printf("  Temperature:  %d 1/100 degrees C\n", temp);
  printf("  Humidity:     %u 0.01%%\n", humi);
  printf("  Light:        %d\n", light);
  printf("  Acceleration: %f\n", accel_mag);
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
  printf("  Random:       %#04x %#04x %#04x %#04x %#04x\n", rand[0], rand[1], rand[2], rand[3], rand[4]);
  printf("  CRC:          %#010lx (%s reference implementation)\n", crc,
         (crc == reference_crc) ? "Matches" : "!! Does not match");
  printf("\n");

  // toggle the blue LED
  libtock_led_toggle(2);
}

int main(void) {
  printf("[Hail] Test App!\n");
  printf("[Hail] Samples all sensors.\n");
  printf("[Hail] Transmits name over BLE.\n");
  printf("[Hail] Button controls LED.\n");

  // Setup BLE
  simple_ble_init(&ble_config);
  simple_adv_only_name();

  // Enable button callbacks
  libtock_button_notify_on_press(0, button_callback);

  // Setup D0, D1, D6, D7
  libtock_gpio_enable_input(0, libtock_pull_down); // D0
  libtock_gpio_enable_input(1, libtock_pull_down); // D1
  libtock_gpio_enable_input(2, libtock_pull_down); // D6
  libtock_gpio_enable_input(3, libtock_pull_down); // D7

  // Enable interrupts on D7
  libtock_gpio_set_interrupt_callback(gpio_callback);
  libtock_gpio_enable_interrupt(3, libtock_change);

  // sample sensors every second
  while (1) {
    sample_sensors();
    delay_ms(1000);
  }
}
