
#include "smtc_hal.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_gpio.h"
#include "lr11xx_hal_context.h"
#include "lr11xx_system.h"

lr11xx_hal_context_t radio_context = {
    .nss    = LR1110_SPI_NSS_PIN,
    .busy   = LR1110_BUSY_PIN,
    .reset  = LR1110_NRESER_PIN,
    .spi_id = 3,
};

void lr1110_sleep_enter( uint32_t tick )
{
    lr11xx_system_version_t version;

    hal_spi_init( );
 
    lr11xx_system_clear_errors( &radio_context );   
    lr11xx_system_set_tcxo_mode( &radio_context , LR11XX_SYSTEM_TCXO_CTRL_3_3V, 50 );
    lr11xx_system_cfg_lfclk( &radio_context , LR11XX_SYSTEM_LFCLK_XTAL, 1 );
    
    lr11xx_system_sleep_cfg_t radio_sleep_cfg;
    radio_sleep_cfg.is_warm_start  = 1;
    radio_sleep_cfg.is_rtc_timeout = 1;
    
    lr11xx_system_drive_dio_in_sleep_mode( &radio_context , true );
    lr11xx_system_set_sleep( &radio_context , radio_sleep_cfg, tick );

    hal_spi_deinit( );
}

int main(void)
{
    hal_pwr_init( );
    
    hal_gpio_init_out( LR1110_SPI_NSS_PIN, HAL_GPIO_SET );
    hal_gpio_init_in( LR1110_BUSY_PIN, HAL_GPIO_PULL_MODE_NONE, HAL_GPIO_IRQ_MODE_OFF, NULL );
    hal_gpio_init_in( LR1110_IRQ_PIN, HAL_GPIO_PULL_MODE_DOWN, HAL_GPIO_IRQ_MODE_RISING, NULL );
    hal_gpio_init_out( LR1110_NRESER_PIN, HAL_GPIO_SET );

    lr1110_sleep_enter( 0 );

    hal_debug_init( );
    PRINTF( "\r\nWX1110 low power\r\n" );
    hal_debug_deinit( );

    hal_mcu_wait_ms( 5000 );
    
    while( 1 )
    {
        nrf_pwr_mgmt_run( );
    }
}
 