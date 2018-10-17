#include "../bosch_port/bosch_port.h"
#include "bme280.h"
#include "bme280_port.h"

#include <console.h>
#include <stdlib.h>
#include <timer.h>

int8_t bme280_port_init(struct bme280_dev *dev){
  dev->dev_id   = BME280_I2C_ADDR_SEC;
  dev->intf     = BME280_I2C_INTF;
  dev->read     = (bme280_com_fptr_t) tock_i2c_read;
  dev->write    = (bme280_com_fptr_t) tock_i2c_write;
  dev->delay_ms = delay_ms;

  return bme280_init(dev);
}

void print_sensor_data(struct bme280_data *comp_data)
{
  char temp[] = "Temperature:       ";
  itoa (comp_data->temperature, &temp[13], 10);
  putnstr(temp, sizeof(temp));

  char press[] = ", Pressure:               ";
  itoa (comp_data->pressure, &press[13], 10);
  putnstr(press, sizeof(press));

  char hum[] = ", Humidity:             ";
  itoa (comp_data->humidity, &hum[13], 10);
  putnstr(hum, sizeof(hum));
  putnstr("\r\n", 2);
}
