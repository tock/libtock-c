#include <stdbool.h>
#include <stdio.h>

#include <libtock-sync/sensors/ambient_light.h>
#include <libtock-sync/sensors/humidity.h>
#include <libtock-sync/sensors/ninedof.h>
#include <libtock-sync/sensors/proximity.h>
#include <libtock-sync/sensors/sound_pressure.h>
#include <libtock-sync/sensors/temperature.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/tock.h>

static libtock_alarm_t alarm;
static bool light          = false;
static bool temperature    = false;
static bool humidity       = false;
static bool ninedof        = false;
static bool ninedof_accel  = false;
static bool ninedof_mag    = false;
static bool ninedof_gyro   = false;
static bool proximity      = false;
static bool sound_pressure = false;
static void alarm_cb(__attribute__ ((unused)) uint32_t now,
                     __attribute__ ((unused)) uint32_t scheduled,
                     __attribute__ ((unused)) void*    opaque) {
  int lite = 0;
  int temp = 0;
  int humi = 0;
  int ninedof_accel_x = 0, ninedof_accel_y = 0, ninedof_accel_z = 0;
  int ninedof_magneto_x = 0, ninedof_magneto_y = 0, ninedof_magneto_z = 0;
  int ninedof_gyro_x = 0, ninedof_gyro_y = 0, ninedof_gyro_z = 0;
  uint8_t prox_reading = 0;
  unsigned char sound_pressure_reading = 0;

  /* *INDENT-OFF* */
  if (light)          libtocksync_ambient_light_read_intensity(&lite);
  if (temperature)    libtocksync_temperature_read(&temp);
  if (humidity)       libtocksync_humidity_read(&humi);
  if (ninedof_accel)  libtocksync_ninedof_read_accelerometer(&ninedof_accel_x, &ninedof_accel_y, &ninedof_accel_z);
  if (ninedof_mag)    libtocksync_ninedof_read_magnetometer(&ninedof_magneto_x, &ninedof_magneto_y, &ninedof_magneto_z);
  if (ninedof_gyro)   libtocksync_ninedof_read_gyroscope(&ninedof_gyro_x, &ninedof_gyro_y, &ninedof_gyro_z);
  if (proximity)      libtocksync_proximity_read(&prox_reading);
  if (sound_pressure) libtocksync_sound_pressure_read(&sound_pressure_reading);

  if (light)          printf("Amb. Light: Light Intensity: %d\n", lite);
  if (temperature)    printf("Temperature:                 %d deg C\n", temp/100);
  if (humidity)       printf("Humidity:                    %u%%\n", humi/100);
  if (ninedof_accel)  printf("Acceleration: X: %d Y: %d Z: %d\n", ninedof_accel_x, ninedof_accel_y, ninedof_accel_z);
  if (ninedof_mag)    printf("Magnetometer: X: %d Y: %d Z: %d\n", ninedof_magneto_x, ninedof_magneto_y, ninedof_magneto_z);
  if (ninedof_gyro)   printf("Gyro:         X: %d Y: %d Z: %d\n", ninedof_gyro_x, ninedof_gyro_y, ninedof_gyro_z);
  if (proximity)      printf("Proximity:                   %u\n", prox_reading);
  if (sound_pressure) printf("Sound Pressure:              %u\n", sound_pressure_reading);

  /* *INDENT-ON* */

  printf("\n");
  libtock_alarm_in_ms(1000, alarm_cb, NULL, &alarm);
}

int main(void) {
  printf("[Sensors] Starting Sensors App.\n");
  printf("[Sensors] All available sensors on the platform will be sampled.\n");

  /* *INDENT-OFF* */
  light          = libtock_ambient_light_exists();
  temperature    = libtock_temperature_exists();
  humidity       = libtock_humidity_exists();
  ninedof        = libtock_ninedof_exists();
  proximity      = libtock_proximity_exists();
  sound_pressure = libtock_sound_pressure_exists();
  /* *INDENT-ON* */

  if (ninedof) {
    int buffer;
    ninedof_accel = (libtocksync_ninedof_read_accelerometer(&buffer, &buffer, &buffer) == RETURNCODE_SUCCESS);
    ninedof_mag   = (libtocksync_ninedof_read_magnetometer(&buffer, &buffer, &buffer) == RETURNCODE_SUCCESS);
    ninedof_gyro  = (libtocksync_ninedof_read_gyroscope(&buffer, &buffer, &buffer) == RETURNCODE_SUCCESS);
  }

  /* *INDENT-OFF* */
  if (light)          printf("[Sensors]   Sampling Ambient Light sensor.\n");
  if (temperature)    printf("[Sensors]   Sampling Temperature sensor.\n");
  if (humidity)       printf("[Sensors]   Sampling Humidity sensor.\n");
  if (ninedof_accel)  printf("[Sensors]   Sampling Accelerometer.\n");
  if (ninedof_mag)    printf("[Sensors]   Sampling Magnetometer.\n");
  if (ninedof_gyro)   printf("[Sensors]   Sampling Gyroscope.\n");
  if (proximity)      printf("[Sensors]   Sampling Proximity sensor.\n");
  if (sound_pressure) printf("[Sensors]   Sampling Sound Pressure sensor.\n");
  /* *INDENT-ON* */

  if (sound_pressure) {
    libtock_sound_pressure_command_enable();
  }

  // Setup periodic alarm to sample the sensors.
  libtock_alarm_in_ms(1000, alarm_cb, NULL, &alarm);

  while (1) {
    yield();
  }
}
