
#include "smtc_hal.h"

static uint8_t SHT41I2cDeviceAddr = 0;

static float temperature = 0;
static float humidity = 0;
static uint8_t temperature_unit = TEMPERATURE_CELSIUS;

static uint8_t crc8( const uint8_t *data, int len );
static void SHT41SetDeviceAddr( uint8_t addr );
static bool SHT41Reset( void );
static bool SHT41WriteBuffer( uint8_t *data, uint8_t size );
static bool SHT41ReadBuffer( uint8_t *data, uint8_t size );
static bool SHT41WriteCommand( uint8_t cmd );
static bool getTemperatureAndHumidity( void );


bool SHT41Test( void )
{
    return true;
}

uint16_t SHT41GetPreheatTime( void )
{
    return SHT41_PREHEAT_TIME_MS;
}

uint16_t SHT41GetPowerOnTime( void )
{
    return SHT41_POWER_ON_TIME_MS;
}

bool SHT41Init( void )
{
    bool ret;
    SHT41SetDeviceAddr( SHT41_I2C_ADDRESS );
    ret = SHT41Reset( );
    hal_mcu_wait_ms( 1 ); // it needs to wait 1ms after soft reset 
    
    return ret;
}

bool SHT41GetTemperature( float *value )
{
    bool ret;
    ret = getTemperatureAndHumidity( );
    if ( ret == true ) 
    {
        if ( temperature_unit == TEMPERATURE_FAHRENHEIT )
        {
            temperature = temperature * UINT_FAHRENHEIT_TO_CELSIUS_A + UINT_FAHRENHEIT_TO_CELSIUS_B * 1000;
            temperature = ( temperature + 5 ) / 100 * 100;

        }
        *value = temperature / 1000;;
    }
    
    return ret;
}

bool SHT41GetHumidity( float *value )
{
    bool ret;
    ret = getTemperatureAndHumidity( );
    if ( ret == true ) *value = humidity / 1000;;
    return ret;
}

bool SHT41GetTempAndHumi( float *temp, float *humi )
{
    bool ret;
    ret = getTemperatureAndHumidity( );
    if ( ret == true ) 
    {
        if ( temperature_unit == TEMPERATURE_FAHRENHEIT )
        {
            temperature = temperature * UINT_FAHRENHEIT_TO_CELSIUS_A + UINT_FAHRENHEIT_TO_CELSIUS_B * 1000;
            temperature = ( temperature + 5 ) / 100 * 100;

        }
        *temp = temperature / 1000;;
        *humi = humidity / 1000;;
    }
    
    return ret;
}

bool SHT41SetTemperatureUnit( uint8_t unit )
{
    if ( unit == TEMPERATURE_FAHRENHEIT || unit == TEMPERATURE_CELSIUS )
    {
        temperature_unit = unit;
        return true;
    }
    else return false;
}

bool SHT41GetTemperatureUnit( uint8_t *unit )
{
    *unit = temperature_unit;
    return true;
}

/* Internal Function */
static bool SHT41Reset( void )
{
    return SHT41WriteCommand( SHT41_SOFTRESET );
}

static bool getTemperatureAndHumidity( void )
{
    uint8_t readbuffer[6] = { 0, };
    uint16_t ST = 0, SRH = 0;
    int64_t temp = 0, humi = 0;
    bool ret;
    uint8_t retry;

    retry = 3;
    while ( retry > 0 )
    {
        ret = SHT41WriteCommand( SHT41_MEAS_HIGHREP_STRETCH );
        if ( ret == true ) break;
        retry --;
        if ( retry == 0 ) return ret;
        hal_mcu_wait_ms( 1 );
    }
    
    hal_mcu_wait_ms( 10 ); // measure time

    ret = SHT41ReadBuffer( readbuffer, 6 );
    if ( ret != true ) return ret;

    ST = readbuffer[0];
    ST <<= 8;
    ST |= readbuffer[1];

    if ( readbuffer[2] != crc8( readbuffer, 2 ) ) return false;

    SRH = readbuffer[3];
    SRH <<= 8;
    SRH |= readbuffer[4];

    if ( readbuffer[5] != crc8( readbuffer + 3, 2 ) ) return false;

    temp = (int64_t)ST * 175 * 1000 / 0xffff - 45 * 1000;
    humi = (int64_t)SRH * 125 * 1000 / 0xffff - 6 * 1000;
    temperature = ( temp + 5 ) / 10 * 10;
    humidity = ( humi + 5 ) / 10 * 10;
    
    return true;
}

static uint8_t crc8( const uint8_t *data, int len )
{
    const uint8_t POLYNOMIAL = 0x31;
    uint8_t crc = 0xFF;

    for(int j = len; j; --j)
    {
        crc ^= *data++;

        for(int i = 8; i; --i)
        {
            crc = (crc & 0x80) ? (crc << 1) ^ POLYNOMIAL : (crc << 1);
        }
    }
    
    return crc; 
}

static bool SHT41WriteCommand( uint8_t cmd ) 
{
    uint8_t buffer[1] = {0, };
    
    buffer[0] = cmd;
    
    return SHT41WriteBuffer( buffer, 1 );
}

static bool SHT41WriteBuffer( uint8_t *data, uint8_t size )
{
    if ( hal_i2c_write_buffer( SHT41I2cDeviceAddr, data, size ) ) return true;
    else return false;
}

static bool SHT41ReadBuffer( uint8_t *data, uint8_t size )
{    
    if ( hal_i2c_read_buffer( SHT41I2cDeviceAddr, data, size ) ) return true;
    else return false;
}

static void SHT41SetDeviceAddr( uint8_t addr )
{
    SHT41I2cDeviceAddr = addr;
}
