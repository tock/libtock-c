
#include "smtc_hal_rtc.h"

#include <libtock/peripherals/syscalls/alarm_syscalls.h>


uint32_t hal_rtc_get_time_s( void )
{
    uint32_t timeInSeconds = hal_rtc_get_time_ms();
    return timeInSeconds / 1000;
}

uint32_t hal_rtc_get_time_ms( void )
{
    uint32_t timeInms = hal_rtc_get_time_100us();
    return timeInms / 10;
}

uint32_t hal_rtc_get_time_100us( void )
{
    uint32_t time = 0;
    uint32_t frequency = 0;
    libtock_alarm_command_read(&time);
    libtock_alarm_command_get_frequency(&frequency);
    uint32_t timeIn100us = (time * 10000) / frequency;
    return timeIn100us;
}

void hal_rtc_wakeup_timer_set_ms( const int32_t milliseconds )
{
    libtocksync_alarm_delay_ms(milliseconds);
}
