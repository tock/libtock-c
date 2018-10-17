#ifndef BME280_PORT_H_
#define BME280_PORT_H_

#include "bme280.h"

#ifdef __cplusplus
extern "C" {
#endif

int8_t bme280_port_init(struct bme280_dev *dev);
void print_sensor_data(struct bme280_data *comp_data);

#ifdef __cplusplus
}
#endif
#endif