
#ifndef __SHT41_H__
#define __SHT41_H__

#include <stdint.h>
#include <stdbool.h>

#define SHT41_I2C_ADDRESS                           0x44    // max power up time 1ms

#define SHT41_MEAS_HIGHREP_STRETCH                  0xFD    // max 8.2ms
#define SHT41_MEAS_MEDREP_STRETCH                   0xF6    // max 4.5ms
#define SHT41_MEAS_LOWREP_STRETCH                   0xE0    // max 1.7ms

#define SHT41_MEAS_WITH_200MW_1S_HEATER             0x39    // max 1.1s
#define SHT41_MEAS_WITH_200MW_01S_HEATER            0x32    // max 0.11s
#define SHT41_MEAS_WITH_110MW_1S_HEATER             0x2F
#define SHT41_MEAS_WITH_110MW_01S_HEATER            0x24
#define SHT41_MEAS_WITH_20MW_1S_HEATER              0x1E
#define SHT41_MEAS_WITH_20MW_01S_HEATER             0x15

#define SHT41_SOFTRESET                             0x94    // max 1ms
#define SHT41_SERIAL_NUM                            0x89

#define SHT41_PREHEAT_TIME_MS                       (1)
#define SHT41_POWER_ON_TIME_MS                      (1)

#define TEMPERATURE_CELSIUS                         'C'
#define TEMPERATURE_FAHRENHEIT                      'F'

#define UINT_FAHRENHEIT_TO_CELSIUS_A                1.8
#define UINT_FAHRENHEIT_TO_CELSIUS_B                32

bool SHT41Test( void );
uint16_t SHT41GetPreheatTime( void );
uint16_t SHT41GetPowerOnTime( void );

/*!
 * \brief Initializes the device
 *
 * \retval status [SENSOR_ERR_OK, SENSOR_ERR_FAIL]
 */
bool SHT41Init( void );
bool SHT41GetTemperature( float *value );
bool SHT41GetHumidity( float *value );
bool SHT41GetTempAndHumi( float *temp, float *humi );

/* SHT41 Config */
bool SHT41SetTemperatureUnit( uint8_t unit );
bool SHT41GetTemperatureUnit( uint8_t *unit );

#endif
