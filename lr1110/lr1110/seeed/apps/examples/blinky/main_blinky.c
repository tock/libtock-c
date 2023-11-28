
#include "smtc_hal.h"

int main(void)
{
    hal_gpio_init_out( USER_LED_G, HAL_GPIO_RESET );
    hal_gpio_init_out( USER_LED_R, HAL_GPIO_RESET );

    while( 1 )
    {
        hal_gpio_init_out( USER_LED_G, HAL_GPIO_SET );
        hal_gpio_init_out( USER_LED_R, HAL_GPIO_RESET );
        hal_mcu_wait_ms( 500 );
        hal_gpio_init_out( USER_LED_G, HAL_GPIO_RESET );
        hal_gpio_init_out( USER_LED_R, HAL_GPIO_SET );
        hal_mcu_wait_ms( 500 );
    }
}
