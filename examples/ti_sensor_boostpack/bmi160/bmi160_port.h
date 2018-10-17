#ifndef BMI160_PORT_H_
#define BMI160_PORT_H_

#include "bmi160.h"
#include "../bmm150/bmm150.h"

#ifdef __cplusplus
extern "C" {
#endif

int8_t bmi160_port_init(struct bmi160_dev *);
void bmi160_print_data(struct bmi160_sensor_data *accel, struct bmi160_sensor_data *gyro);

#ifdef __cplusplus
}
#endif
#endif