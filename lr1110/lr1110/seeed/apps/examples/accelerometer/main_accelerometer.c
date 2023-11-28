
#include "smtc_hal.h"

int main(void)
{
    hal_debug_init( );
    hal_i2c_master_init( );
    hal_gpio_init_out( SENSOR_POWER, HAL_GPIO_SET );
    hal_mcu_wait_ms( 10 ); // wait power on
    LIS3dInit( );

    PRINTF( "Accelerometer test\r\n" );

    while( 1 )
    {
        float ax = 0, ay = 0, az = 0;
        LIS3dGetAcceleration( &ax, &ay, &az );
        PRINTF( "%.3f, %.3f, %.3f\r\n", ax, ay, az );
        hal_mcu_wait_ms( 1000 );
    }
}
