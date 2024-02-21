#include "smtc_hal.h"

static int16_t accRange;

static void readRegisterRegion( uint8_t *outputPointer, uint8_t reg, uint8_t length );
static void writeRegister( uint8_t reg, uint8_t val );
static uint8_t readRegister( uint8_t reg );
static uint16_t readRegisterInt16( uint8_t reg );

void LIS3dInit( void )
{
    uint8_t config5 = LIS3DHTR_REG_TEMP_ADC_PD_ENABLED |
                      LIS3DHTR_REG_TEMP_TEMP_EN_DISABLED;

    writeRegister( LIS3DHTR_REG_TEMP_CFG, config5 );
    hal_mcu_wait_ms( LIS3DHTR_CONVERSIONDELAY );

    uint8_t config1 = LIS3DHTR_REG_ACCEL_CTRL_REG1_LPEN_NORMAL | // Normal Mode
                      LIS3DHTR_REG_ACCEL_CTRL_REG1_AZEN_ENABLE | // Acceleration Z-Axis Enabled
                      LIS3DHTR_REG_ACCEL_CTRL_REG1_AYEN_ENABLE | // Acceleration Y-Axis Enabled
                      LIS3DHTR_REG_ACCEL_CTRL_REG1_AXEN_ENABLE;

    writeRegister( LIS3DHTR_REG_ACCEL_CTRL_REG1, config1 );
    hal_mcu_wait_ms( LIS3DHTR_CONVERSIONDELAY );

    uint8_t config4 = LIS3DHTR_REG_ACCEL_CTRL_REG4_BDU_NOTUPDATED | // Continuous Update
                      LIS3DHTR_REG_ACCEL_CTRL_REG4_BLE_LSB |        // Data LSB @ lower address
                      LIS3DHTR_REG_ACCEL_CTRL_REG4_HS_DISABLE |      // High Resolution Disable
                      LIS3DHTR_REG_ACCEL_CTRL_REG4_ST_NORMAL |      // Normal Mode
                      LIS3DHTR_REG_ACCEL_CTRL_REG4_SIM_4WIRE;       // 4-Wire Interface

    writeRegister( LIS3DHTR_REG_ACCEL_CTRL_REG4, config4 );

    hal_mcu_wait_ms( LIS3DHTR_CONVERSIONDELAY );

    LIS3dSetFullScaleRange( LIS3DHTR_RANGE_16G );
    LIS3dSetOutputDataRate( LIS3DHTR_DATARATE_400HZ );
}

bool LIS3dIsConnection( void )
{
    return ( LIS3dGetDeviceID( ) == 0x33 );
}

void LIS3dSetPowerMode( power_type_t mode )
{
    uint8_t data = 0;

    data = readRegister( LIS3DHTR_REG_ACCEL_CTRL_REG1 );

    data &= ~LIS3DHTR_REG_ACCEL_CTRL_REG1_LPEN_MASK;
    data |= mode;

    writeRegister( LIS3DHTR_REG_ACCEL_CTRL_REG1, data );
    hal_mcu_wait_ms( LIS3DHTR_CONVERSIONDELAY );
}

void LIS3dSetFullScaleRange( scale_type_t range )
{
    uint8_t data = 0;

    data = readRegister( LIS3DHTR_REG_ACCEL_CTRL_REG4 );

    data &= ~LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_MASK;
    data |= range;

    writeRegister( LIS3DHTR_REG_ACCEL_CTRL_REG4, data );
    hal_mcu_wait_ms( LIS3DHTR_CONVERSIONDELAY );

    switch( range )
    {
        case LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_16G:
            accRange = 1280;
        break;
        case LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_8G:
            accRange = 3968;
        break;
        case LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_4G:
            accRange = 7282;
        break;
        case LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_2G:
            accRange = 16000;
        break;
        default:
        break;
    }
}

void LIS3dSetOutputDataRate( odr_type_t odr )
{
    uint8_t data = 0;

    data = readRegister( LIS3DHTR_REG_ACCEL_CTRL_REG1 );

    data &= ~LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_MASK;
    data |= odr;

    writeRegister( LIS3DHTR_REG_ACCEL_CTRL_REG1, data );
    hal_mcu_wait_ms( LIS3DHTR_CONVERSIONDELAY );
}

void LIS3dSetHighSolution( bool enable )
{
    uint8_t data = 0;
    data = readRegister( LIS3DHTR_REG_ACCEL_CTRL_REG4 );
   
    data = enable? data | LIS3DHTR_REG_ACCEL_CTRL_REG4_HS_ENABLE : data & ~LIS3DHTR_REG_ACCEL_CTRL_REG4_HS_ENABLE;
    
    writeRegister( LIS3DHTR_REG_ACCEL_CTRL_REG4, data );
    return; 
}

bool LIS3dAvailable( )
{
    uint8_t status = 0;
    status = readRegister( LIS3DHTR_REG_ACCEL_STATUS2 );
    status &= LIS3DHTR_REG_ACCEL_STATUS2_UPDATE_MASK;
    return status;
}

void LIS3dGetAcceleration( float *x, float *y, float *z )
{
    // Read the Accelerometer
    uint8_t buf[8]={0};

    // Read the Data
    readRegisterRegion(buf,LIS3DHTR_REG_ACCEL_OUT_X_L,6);

    // Conversion of the result
    // 16-bit signed result for X-Axis Acceleration Data of LIS3DHTR
    *x = (float)((int16_t*)buf)[0] / accRange;
    // 16-bit signed result for Y-Axis Acceleration Data of LIS3DHTR
    *y = (float)((int16_t*)buf)[1] / accRange;
    // 16-bit signed result for Z-Axis Acceleration Data of LIS3DHTR
    *z = (float)((int16_t*)buf)[2] / accRange;
}

float LIS3dGetAccelerationX( void )
{
    // Read the Accelerometer
    uint8_t xAccelLo, xAccelHi;
    int16_t x;

    // Read the Data
    // Reading the Low X-Axis Acceleration Data Register
    xAccelLo = readRegister(LIS3DHTR_REG_ACCEL_OUT_X_L);
    // Reading the High X-Axis Acceleration Data Register
    xAccelHi = readRegister(LIS3DHTR_REG_ACCEL_OUT_X_H);
    // Conversion of the result
    // 16-bit signed result for X-Axis Acceleration Data of LIS3DHTR
    x = (int16_t)((xAccelHi << 8) | xAccelLo);

    return (float)x / accRange;
}

float LIS3dGetAccelerationY( void )
{
    // Read the Accelerometer
    uint8_t yAccelLo, yAccelHi;
    int16_t y;

    // Reading the Low Y-Axis Acceleration Data Register
    yAccelLo = readRegister(LIS3DHTR_REG_ACCEL_OUT_Y_L);
    // Reading the High Y-Axis Acceleration Data Register
    yAccelHi = readRegister(LIS3DHTR_REG_ACCEL_OUT_Y_H);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Acceleration Data of LIS3DHTR
    y = (int16_t)((yAccelHi << 8) | yAccelLo);

    return (float)y / accRange;
}

float LIS3dGetAccelerationZ( void )
{
    // Read the Accelerometer
    uint8_t zAccelLo, zAccelHi;
    int16_t z;

    // Reading the Low Z-Axis Acceleration Data Register
    zAccelLo = readRegister(LIS3DHTR_REG_ACCEL_OUT_Z_L);
    // Reading the High Z-Axis Acceleration Data Register
    zAccelHi = readRegister(LIS3DHTR_REG_ACCEL_OUT_Z_H);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Acceleration Data of LIS3DHTR
    z = (int16_t)((zAccelHi << 8) | zAccelLo);

    return (float)z / accRange;
}

void LIS3dClick( uint8_t c, uint8_t click_thresh, uint8_t limit, uint8_t latency, uint8_t window )
{
    if( !c )
    {
        uint8_t r = readRegister( LIS3DHTR_REG_ACCEL_CTRL_REG3 );
        r &= ~(0x80); // turn off I1_CLICK
        writeRegister( LIS3DHTR_REG_ACCEL_CTRL_REG3, r );
        writeRegister( LIS3DHTR_REG_ACCEL_CLICK_CFG, 0 );
        return;
    }
    writeRegister( LIS3DHTR_REG_ACCEL_CTRL_REG3, 0x80 );
    writeRegister( LIS3DHTR_REG_ACCEL_CTRL_REG5, 0x08 );

    if (c == 1)
    {
        writeRegister( LIS3DHTR_REG_ACCEL_CLICK_CFG, 0x15 );
    }
    if (c == 2)
    {
        writeRegister( LIS3DHTR_REG_ACCEL_CLICK_CFG, 0x2A );
    }

    writeRegister( LIS3DHTR_REG_ACCEL_CLICK_THS, click_thresh );
    writeRegister( LIS3DHTR_REG_ACCEL_TIME_LIMIT, limit );
    writeRegister( LIS3DHTR_REG_ACCEL_TIME_LATENCY, latency );
    writeRegister( LIS3DHTR_REG_ACCEL_TIME_WINDOW, window );
}

void LIS3dOpenTemp( )
{
    uint8_t config5 = LIS3DHTR_REG_TEMP_ADC_PD_ENABLED |
                      LIS3DHTR_REG_TEMP_TEMP_EN_ENABLED;

    writeRegister( LIS3DHTR_REG_TEMP_CFG, config5 );
    hal_mcu_wait_ms( LIS3DHTR_CONVERSIONDELAY );
}

void LIS3dCloseTemp( )
{
    uint8_t config5 = LIS3DHTR_REG_TEMP_ADC_PD_ENABLED |
                      LIS3DHTR_REG_TEMP_TEMP_EN_DISABLED;

    writeRegister( LIS3DHTR_REG_TEMP_CFG, config5 );
    delay( LIS3DHTR_CONVERSIONDELAY );
}

uint16_t LIS3dReadbitADC1( void )
{
    uint8_t adc1_l, adc1_h;
    int16_t intTemp;
    uint16_t uintTemp;
    adc1_l = readRegister( 0x08 );
    adc1_h = readRegister( 0x09 );

    intTemp = (int16_t)(adc1_h << 8) | adc1_l;
    intTemp = 0 - intTemp;
    uintTemp = intTemp + 32768;
    return uintTemp >> 6;
}

uint16_t LIS3dReadbitADC2( void )
{
    uint8_t adc2_l, adc2_h;
    int16_t intTemp;
    uint16_t uintTemp;
    adc2_l = readRegister( 0x0A );
    adc2_h = readRegister( 0x0B );
    intTemp = (int16_t)(adc2_h << 8) | adc2_l;
    intTemp = 0 - intTemp;
    uintTemp = intTemp + 32768;
    return uintTemp >> 6;
}

uint16_t LIS3dReadbitADC3( void )
{
    uint8_t adc3_l, adc3_h;
    int16_t intTemp;
    uint16_t uintTemp;
    adc3_l = readRegister( 0x0C );
    adc3_h = readRegister( 0x0D );

    intTemp = (int16_t)(adc3_h << 8) | adc3_l;
    intTemp = 0 - intTemp;
    uintTemp = intTemp + 32768;
    return uintTemp >> 6;
}

int16_t LIS3dGetTemperature( void )
{
    int16_t result = (( int16_t )readRegisterInt16( 0x0c )) / 256;
    result += 25;
    return result;
}

uint8_t LIS3dGetDeviceID( void )
{
    return readRegister( LIS3DHTR_REG_ACCEL_WHO_AM_I );
}

void LIS3dReset( void )
{
    // TODO
}

void readRegisterRegion( uint8_t *outputPointer, uint8_t reg, uint8_t length )
{
    reg |= 0x80;
    hal_i2c_write_buffer( LIS3DHTR_DEFAULT_ADDRESS, &reg, 1 );
    hal_i2c_read_buffer( LIS3DHTR_DEFAULT_ADDRESS, outputPointer, length );
}

void writeRegister( uint8_t reg, uint8_t val )
{
    uint8_t buf[4] = { 0 };
    buf[0] = reg;
    buf[1] = val;
    hal_i2c_write_buffer( LIS3DHTR_DEFAULT_ADDRESS, buf, 2 );
}

uint8_t readRegister( uint8_t reg )
{
    uint8_t data;
    readRegisterRegion( &data, reg, 1 );
    return data;
}

uint16_t readRegisterInt16( uint8_t reg )
{
    uint8_t buf[2];
    uint16_t output = 0;
    readRegisterRegion( buf, reg, 2 );
    output = buf[0] | (uint16_t) ( buf[1] << 8 );
    return output;
}
