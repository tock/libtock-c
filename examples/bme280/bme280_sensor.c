#include "bme280_sensor.h"

// system includes


// user includes
#include "bme280_common.h"


/**
 * @brief Required time between measurements
 * 
 * @see BME280Init
 */
static uint32_t period = 0;

/**
 * @brief Device definition
 * 
 * @see BME280Init
 */
static struct bme280_dev dev;

/**
 * @brief Device settings
 * 
 * @see BME280Init
 */
static struct bme280_settings settings;


BME280Status BME280Init(void) {
  int8_t rslt;

  /* Interface selection is to be updated as parameter
   * For I2C :  BME280_I2C_INTF
   * For SPI :  BME280_SPI_INTF
   */
  rslt = bme280_interface_selection(&dev, BME280_I2C_INTF);
  if (rslt != BME280_OK) {
    return rslt;
  }

  rslt = bme280_init(&dev);
  if (rslt != BME280_OK) {
    return rslt;
  }

  /* Always read the current settings before writing, especially when all the configuration is not modified */
  rslt = bme280_get_sensor_settings(&settings, &dev);
  if (rslt != BME280_OK) {
    return rslt;
  }

  /* Configuring the over-sampling rate, filter coefficient and standby time */
  /* Overwrite the desired settings */
  settings.filter = BME280_FILTER_COEFF_OFF;

  /* Over-sampling rate for humidity, temperature and pressure */
  settings.osr_h = BME280_OVERSAMPLING_1X;
  settings.osr_p = BME280_OVERSAMPLING_1X;
  settings.osr_t = BME280_OVERSAMPLING_1X;

  /* Setting the standby time */
  settings.standby_time = BME280_STANDBY_TIME_0_5_MS;

  rslt = bme280_set_sensor_settings(BME280_SEL_ALL_SETTINGS, &settings, &dev);
  if (rslt != BME280_OK) {
    return rslt;
  }

  /* Always set the power mode after setting the configuration */
  rslt = bme280_set_sensor_mode(BME280_POWERMODE_FORCED, &dev);
  if (rslt != BME280_OK) {
    return rslt;
  }

  /* Calculate measurement time in microseconds */
  rslt = bme280_cal_meas_delay(&period, &settings);
  if (rslt != BME280_OK) {
    return rslt;
  }

  return rslt;
}

BME280Status BME280MeasureAll(BME280Data *data) {
  int8_t rslt = BME280_E_NULL_PTR;
  uint8_t status_reg;
 
  // trigger measurement
  rslt = bme280_set_sensor_mode(BME280_POWERMODE_FORCED, &dev);
  if (rslt != BME280_OK) {
    return rslt;
  }

  // check the status
  rslt = bme280_get_regs(BME280_REG_STATUS, &status_reg, 1, &dev);
  if (rslt != BME280_OK) {
    return rslt;
  }

  if (status_reg & BME280_STATUS_MEAS_DONE)
  {
    /* Measurement time delay given to read sample */
    dev.delay_us(period, dev.intf_ptr);

    /* Read compensated data */
    rslt = bme280_get_sensor_data(BME280_ALL, data, &dev);
    if (rslt != BME280_OK) {
      return rslt;
    }
  }

  // adjust based on defines
#ifndef BME280_DOUBLE_ENABLE
/*
  data->temperature = data->temperature / 100;
  data->humidity = data->humidity / 1000;
  */
  data->temperature = data->temperature;
  data->humidity = data->humidity;
#endif
    
#ifdef BME280_64BIT_ENABLE 
  data->pressure = data->pressure / 100;
#endif

  return rslt;
}
