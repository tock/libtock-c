/**
 * @file bme280_sensor.h
 * @author John Madden (jmadden173@pm.me)
 * @brief Sensor interface for the BME280 sensor
 * @version 0.1
 * @date 2024-05-17
 * 
 * @copyright Copyright (c) 2024
 * 
 * This is the user code file that allows for interaction with the BME280
 * sensor. It provides a function for reading each measurement and a function
 * for a function that can be integrated with the sensors api.
 * 
 * @see sensors.h
 */

#ifndef __BME280_SENSOR_H__
#define __BME280_SENSOR_H__

#include <stdint.h>
#include <stddef.h>

//#include "i2c.h"
//#include "stm32wlxx_hal_i2c.h"
//#include "stm32wlxx_hal_def.h"
//#include "stm32_systime.h"

#include "bme280.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Struct to store data from sensor
 * 
 */
typedef struct bme280_data BME280Data;

/**
 * @brief BME280 error codes
 * 
 * The enum constants are defined to match error codes in bme280_defs.h
 */
typedef enum {
  BME280_STATUS_OK = BME280_OK,
  BME280_STATUS_NULL_PTR = BME280_E_NULL_PTR,
  BME280_STATUS_COMM_FAIL = BME280_E_COMM_FAIL,
  BME280_STATUS_INVALID_LEN = BME280_E_INVALID_LEN,
  BME280_STATUS_DEV_NOT_FOUND = BME280_E_DEV_NOT_FOUND,
  BME280_STATUS_SLEEP_MODE_FAIL = BME280_E_SLEEP_MODE_FAIL,
  BME280_STATUS_NVM_COPY_FAILED = BME280_E_NVM_COPY_FAILED,
  BME280_STATUS_ERROR = -7
} BME280Status;

BME280Status BME280Init(void);

BME280Status BME280Deinit(void);

/**
 * @brief Measures temperature, pressure, and humidity
 * 
 * If you only need a single measurement channel, take all measurements and only
 * take the needed measurement. Internally all channels need to be measured to
 * get the value of a single channel.
 * 
 * @param data 
 * @return Status code
 */
BME280Status BME280MeasureAll(BME280Data *data);


#ifdef __cplusplus
}
#endif

#endif /* __BME280_H */