#include <libtock-sync/services/alarm.h>
#include <libtock/interface/led.h>
#include <stdio.h>
#include "bme280_sensor.h"
#include <string.h>
#include <libtock/interface/console.h>

char hello[] = "Hello World!\r\n";

static void nop(
  returncode_t ret __attribute__((unused)),
  uint32_t     bytes_written __attribute__((unused))) {}


int main(void) {
  
  printf("bme280\n");
  BME280Status status = BME280Init();
  while (1) {
    // Sleep
    libtocksync_alarm_delay_ms(500);

    BME280Data data = {};

    status = BME280MeasureAll(&data);
    if (status != BME280_STATUS_OK) {
      printf("BME280 Error, status: %d\r\n", status);
      continue;
    }

    printf("Pressure: %u, Temperature: %d, Humidity: %u\r\n", data.pressure,
            data.temperature, data.humidity);
  }
}

