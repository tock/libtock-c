// #include "nrf_pwr_mgmt.h"
// #include "nrf_gpio.h"

#include <wm1110/wm1110.h>
#include <timer.h>
#include <stdio.h>

#define LR1110_FW_VERSION 0x0307
#define LR1110_FW_TYPE 0x01
#define LR1120_FW_VERSION 0x0101
#define LR1120_FW_TYPE 0x02


lr11xx_hal_context_t radio_context = {
    .nss    = LR1110_SPI_NSS_PIN,
    .busy   = LR1110_BUSY_PIN,
    .reset  = LR1110_NRESER_PIN,
    .spi_id = 3,
};

void lr1110_sleep_enter( uint32_t tick )
{
    lr11xx_system_version_t version;
    lr11xx_status_t status;

    hal_spi_init( );

    printf("\nafter spi init\n");

    lr11xx_hal_reset(&radio_context);

    lr11xx_system_clear_errors( &radio_context );
    // lr11xx_system_set_tcxo_mode( &radio_context , LR11XX_SYSTEM_TCXO_CTRL_3_3V, 50 );
    // lr11xx_system_cfg_lfclk( &radio_context , LR11XX_SYSTEM_LFCLK_XTAL, 1 );

    // lr11xx_system_sleep_cfg_t radio_sleep_cfg;
    // radio_sleep_cfg.is_warm_start  = 1;
    // radio_sleep_cfg.is_rtc_timeout = 1;

    // lr11xx_system_drive_dio_in_sleep_mode( &radio_context , true );
    // lr11xx_system_set_sleep( &radio_context , radio_sleep_cfg, tick );

    status = lr11xx_system_get_version( &radio_context, &version );
    printf("Hardware Version: %u, 0x%04X\n", version.hw, version.hw);  // 4
    printf("Type: %u, 0x%04X\n", version.type, version.type); // 4
    printf("Firmware Version: %u, 0x%04X\n", version.fw, version.fw); // 1028, 0x0404
    if( status != LR11XX_STATUS_OK )
    {
        printf( "Failed to get LR11XX firmware version\n" );
    }
    if( ( ( version.fw != LR1110_FW_VERSION ) && ( version.type = LR1110_FW_TYPE ) ) &&
        ( ( version.fw != LR1120_FW_VERSION ) && ( version.type = LR1120_FW_TYPE ) ) )
    {
        printf( "Wrong LR11XX firmware version, expected 0x%04X, got 0x%04X\n", LR1110_FW_VERSION,
                             version.fw );
    }

    lr11xx_system_uid_t unique_identifier;
    status= lr11xx_system_read_uid( &radio_context, &unique_identifier );

    printf("uid %x %x %x\n", unique_identifier[0], unique_identifier[1], unique_identifier[2]);




    // printf("\nbefore spi deinit\n");
    // hal_spi_deinit( );
}

int main(void)
{
    //hal_pwr_init( );
    printf("\nbefore initalize lora\n");

    hal_gpio_init_out( LR1110_SPI_NSS_PIN, HAL_GPIO_SET );
    hal_gpio_init_in( LR1110_BUSY_PIN, HAL_GPIO_PULL_MODE_NONE, HAL_GPIO_IRQ_MODE_OFF, NULL );
    hal_gpio_init_in( LR1110_IRQ_PIN, HAL_GPIO_PULL_MODE_DOWN, HAL_GPIO_IRQ_MODE_RISING, NULL );
    hal_gpio_init_out( LR1110_NRESER_PIN, HAL_GPIO_SET );

    printf("\nbefore sleep enter\n");

    lr1110_sleep_enter( 0 );

    //hal_debug_init( );
    printf( "\r\nWX1110 low power\r\n" );
    //hal_debug_deinit( );

    delay_ms( 5000 );

    // while( 1 )
    // {
    //     nrf_pwr_mgmt_run( );
    // }
}
