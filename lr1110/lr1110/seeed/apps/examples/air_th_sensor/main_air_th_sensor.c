
#include "smtc_hal.h"

int main(void)
{
    hal_debug_init( );
    hal_i2c_master_init( );
    hal_gpio_init_out( SENSOR_POWER, HAL_GPIO_SET );
    hal_mcu_wait_ms( 10 ); // wait power on
    SHT41Init( );
    PRINTF( "SHT41 test\r\n" );
    
    while( 1 )
    {        
        float temp = 0, humi = 0;
        SHT41GetTempAndHumi( &temp, &humi );
        PRINTF( "temp = %.1f, humi = %.1f\r\n", temp, humi );
        hal_mcu_wait_ms( 1000 );
    }
}
