
#include "nrf_nvic.h"
#include "nrf52840.h"
#include "smtc_hal.h"
#include "nrf_pwr_mgmt.h"
#include "smtc_hal_rtc.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "hardfault.h"

void hal_clock_init(void)
{
    nrf_drv_clock_init();
    nrf_drv_clock_lfclk_request( NULL );
}

void hal_pwr_init(void)
{
    nrf_pwr_mgmt_init( );
}

void hal_mcu_init( void )
{
    hal_clock_init( );
    hal_pwr_init( );
    hal_flash_init( );
    hal_gpio_init( );
    hal_rtc_init( );
    hal_lp_timer_init( );
    hal_debug_init( );
    hal_spi_init( );
    hal_rng_init( );
}

void hal_mcu_disable_irq( void )
{
    __disable_irq();
}

void hal_mcu_enable_irq( void )
{
    __enable_irq();
}

void hal_mcu_reset( void )
{
    sd_nvic_SystemReset( );
}

void __attribute__( ( optimize( "O0" ) ) ) hal_mcu_wait_us( const int32_t microseconds )
{
    // Work @64MHz
    const uint32_t nb_nop = microseconds * 1000 / 171;
    for( uint32_t i = 0; i < nb_nop; i++ )
    {
        __NOP( );
    }
}
void hal_mcu_wait_ms( const int32_t ms )
{
    for( uint32_t i = 0; i < ms; i++ )
        hal_mcu_wait_us( 1000 );
}

void hal_mcu_partial_sleep_enable( bool enable )
{
	
}

void hal_mcu_set_sleep_for_ms( const int32_t milliseconds )
{
    bool last_sleep_loop = false;
    int32_t time_counter = milliseconds;

    if( milliseconds <= 0 ) return;

    do
    {
        int32_t time_sleep = 0;
        float time_sleep_max = RTC_2_PER_TICK * RTC_2_MAX_TICKS;
        if( time_counter > time_sleep_max )
        {
            time_sleep = time_sleep_max;
            time_counter -= time_sleep;
        }
        else
        {
            time_sleep = time_counter;
            last_sleep_loop = true;
        }

        if( time_sleep > 50 )
        {
            hal_rtc_wakeup_timer_set_ms( time_sleep );
            nrf_pwr_mgmt_run( );
        }
    } while( last_sleep_loop == false );
}

static char string[HAL_PRINT_BUFFER_SIZE];
static void vprint( const char* fmt, va_list argp )
{
    if( 0 < vsprintf( string, fmt, argp ) )  // build string
    {
        hal_debug_init( );
        hal_uart_tx(( uint8_t* ) string, strlen( string ) );
        hal_debug_deinit( );
    }
}

void hal_mcu_trace_print( const char* fmt, ... )
{
    va_list argp;
    va_start( argp, fmt );
    vprint( fmt, argp );
    va_end( argp );
}

void hal_hex_to_bin( char *input, uint8_t *dst, int len )
{
	char tmp[3];
	uint16_t length = strlen( input );
	tmp[2] = NULL;
	for( int i = 0; i < length; i+=2 )
	{
		tmp[0] = input[i];
		tmp[1] = input[i+1];
		dst[i/2] = ( uint8_t )strtol(( const char * )tmp, NULL, 16 );
		if (i >= (2 * len )) break;
	}
}

void hal_print_bin_to_hex( uint8_t *buf, uint16_t len )
{
    for( uint16_t i = 0; i < len; i++ )
    {
        PRINTF( "%02X", buf[i] );
    }
    PRINTF( "\r\n" );
}
