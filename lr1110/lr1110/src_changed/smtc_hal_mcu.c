
#include "smtc_hal.h"
#include "smtc_hal_rtc.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/services/alarm.h>

void hal_clock_init(void)
{
}

void hal_pwr_init(void)
{
}

void hal_mcu_init( void )
{
    hal_clock_init( );
    hal_pwr_init( );
    hal_flash_init( );
    hal_gpio_init( );
    hal_spi_init( );
}

void hal_mcu_reset( void )
{
}

void __attribute__( ( optimize( "O0" ) ) ) hal_mcu_wait_us( const int32_t microseconds )
{
    // Work @64MHz
    uint32_t dummy = 0;
    const uint32_t nb_nop = microseconds * 1000 / 171;
    for( uint32_t i = 0; i < nb_nop; i++ )
    {
        //__NOP( );
    }
}
void hal_mcu_wait_ms( const int32_t ms )
{
    libtocksync_alarm_delay_ms(ms);
}

void hal_mcu_set_sleep_for_ms( const int32_t milliseconds )
{
    bool last_sleep_loop = false;
    int32_t time_counter = milliseconds;

    if( milliseconds <= 0 ) return;

    hal_rtc_wakeup_timer_set_ms( milliseconds );
}

void hal_hex_to_bin( char *input, uint8_t *dst, int len )
{
	char tmp[3];
	uint16_t length = strlen( input );
	tmp[2] = '\0';
	for( int i = 0; i < length; i+=2 )
	{
		tmp[0] = input[i];
		tmp[1] = input[i+1];
		dst[i/2] = ( uint8_t )strtol(( const char * )tmp, NULL, 16 );
		if (i >= (2 * len )) break;
	}
}
