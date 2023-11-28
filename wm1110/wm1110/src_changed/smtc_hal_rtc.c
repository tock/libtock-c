
//#include "nrf_drv_rtc.h"
//#include "nrf_drv_clock.h"
#include "smtc_hal_rtc.h"
//#include "smtc_hal_dbg_trace.h"

// static const nrf_drv_rtc_t rtc_2 = NRF_DRV_RTC_INSTANCE( 2 );
// static nrf_drv_rtc_config_t rtc_config_2 = NRF_DRV_RTC_DEFAULT_CONFIG;
// static uint32_t rtc_2_tick_diff = 0;

// static void rtc_2_handler( nrf_drv_rtc_int_type_t int_type )
// {    
//     if( int_type == NRF_DRV_RTC_INT_OVERFLOW )
//     {
//         rtc_2_tick_diff += RTC_2_MAX_TICKS;
//     }
// }

// void hal_rtc_init( void )
// {
//     rtc_config_2.prescaler = 2; // base tick is 0.091552734375 ms
//     nrf_drv_rtc_init( &rtc_2, &rtc_config_2, rtc_2_handler );
//     nrf_drv_rtc_overflow_enable( &rtc_2,true );
//     nrf_drv_rtc_enable( &rtc_2 );
// }

uint32_t hal_rtc_get_time_s( void )
{
    // double temp = nrf_drv_rtc_counter_get( &rtc_2 ) + rtc_2_tick_diff;
    // return temp * RTC_2_PER_TICK / 1000;

    uint32_t timeInSeconds = hal_rtc_get_time_ms();
    return timeInSeconds / 1000;
}

uint32_t hal_rtc_get_time_ms( void )
{
    // double temp = nrf_drv_rtc_counter_get( &rtc_2 ) + rtc_2_tick_diff;
    // return temp * RTC_2_PER_TICK;

    uint32_t timeInms = hal_rtc_get_time_100us();
    return timeInms / 10;
}

uint32_t hal_rtc_get_time_100us( void )
{
    // double temp = nrf_drv_rtc_counter_get( &rtc_2 ) + rtc_2_tick_diff;
    // return temp * RTC_2_PER_TICK * 10;
    
    uint32_t time = 0;
    uint32_t frequency = 0;
    alarm_internal_read(&time);
    alarm_internal_frequency(&frequency);
    uint32_t timeIn100us = (time * 10000) / frequency;
    return timeIn100us;
}

// uint32_t hal_rtc_get_max_ticks( void )
// {
// 	return nrf_drv_rtc_max_ticks_get( &rtc_2 );
// }

void hal_rtc_wakeup_timer_set_ms( const int32_t milliseconds )
{
    delay_ms(milliseconds);
    
    // uint32_t temp = 0;
    // float temp_f = milliseconds;
    // temp_f = temp_f / RTC_2_PER_TICK + 1;
    // temp = nrf_drv_rtc_counter_get( &rtc_2 ) + temp_f;
    // nrf_drv_rtc_cc_set( &rtc_2, 0, temp & RTC_2_MAX_TICKS, true );

    // uint32_t frequency = 0;
    // uint32_t current_time = 0;
    // uint32_t ticks = 0;
    // alarm_internal_read(&current_time);
    // alarm_internal_frequency(&frequency);
    // ticks = (uint32_t)(((float)milliseconds / 1000) * frequency);
    // alarm_internal_set(current_time, ticks);
}

// void hal_rtc_wakeup_timer_stop( void )
// {
	
// }
