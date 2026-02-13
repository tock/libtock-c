#include <libtock-sync/services/alarm.h>
#include <libtock/interface/led.h>
#include <stdio.h>
#include "bme280_sensor.h"


int main(void) {
 while (1) {
    // Sleep
    libtocksync_alarm_delay_ms(500);

    BME280Data data = {};

    BME280Status status = BME280MeasureAll(&data);
    if (status != BME280_STATUS_OK) {
      printf("BME280 Error, status: %d\r\n", status);
      continue;
    }

    printf("Pressure: %u, Temperature: %d, Humidity: %u\r\n", data.pressure,
            data.temperature, data.humidity);
  }
}

