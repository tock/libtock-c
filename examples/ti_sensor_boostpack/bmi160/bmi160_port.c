#include "../bosch_port/bosch_port.h"
#include "bmi160.h"
#include "bmi160_port.h"

#include <console.h>
#include <stdlib.h>
#include <timer.h>

int8_t bmi160_port_init(struct bmi160_dev *dev){
  dev->id        = BMI160_I2C_ADDR | 0b1;
  dev->interface = BMI160_I2C_INTF;
  dev->read      = (bmi160_com_fptr_t) tock_i2c_read;
  dev->write     = (bmi160_com_fptr_t) tock_i2c_write;
  dev->delay_ms  = delay_ms;

  return bmi160_init(dev);
}

void print_data(struct bmi160_sensor_data *data);
void print_data(struct bmi160_sensor_data *data){
  char x[] = "x:       ";
  itoa (data->x, &x[3], 10);
  putnstr(x, sizeof(x));

  char y[] = ", y:               ";
  itoa (data->y, &y[5], 10);
  putnstr(y, sizeof(y));

  char z[] = ", z:             ";
  itoa (data->z, &z[5], 10);
  putnstr(z, sizeof(z));
}

void bmi160_print_data(struct bmi160_sensor_data *accel, struct bmi160_sensor_data *gyro){
  char accel_str[] = "BMI160 Accel ";
  putnstr(accel_str, sizeof(accel_str));
  print_data(accel);
  putnstr("\r\n", 2);

  char gyro_str[] = "BMI160 Gyro  ";
  putnstr(gyro_str, sizeof(gyro_str));
  print_data(gyro);
  putnstr("\r\n", 2);
}

