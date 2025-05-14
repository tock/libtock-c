
#include "smtc_hal_rtc.h"

#include <libtock/peripherals/syscalls/alarm_syscalls.h>
#include <libtock-sync/services/alarm.h>


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

static uint32_t ticks_to_100us(uint32_t ticks) {
    // `ticks_to_ms`'s conversion will be accurate to within the range
    // 0 to 1 milliseconds less than the exact conversion
    // (true millisecond conversion - [0,1) milliseconds).

    uint32_t frequency;
    libtock_alarm_command_get_frequency(&frequency);

    uint64_t seconds = (ticks / frequency);
    uint64_t hundredus_per_second = 10000;

    // Calculate the conversion of full seconds to ticks.
    uint64_t hundredus = seconds * hundredus_per_second;

    // To get conversion accuracy within 1 millisecond, the conversion
    // must also convert partial seconds.
    uint64_t leftover_ticks = ticks % frequency;

    // This calculation is mathematically equivalent to doing:
    //
    //   `leftover_ticks` / (`frequency` / `hundredus_per_second`)
    //
    // This is taking the remaining unconverted ticks (leftover_ticks)
    // and dividing by the number of ticks per millisecond
    // (`frequency` (ticks per second) / `1000` hundredus per second)
    // The division is done this way because of the same argument in
    // `ms_to_ticks`.
    hundredus += (leftover_ticks * hundredus_per_second) / frequency;

    return hundredus;
}

uint32_t hal_rtc_get_time_100us( void )
{
    uint32_t time = 0;
    libtock_alarm_command_read(&time);
    return ticks_to_100us(time);
}

void hal_rtc_wakeup_timer_set_ms( const int32_t milliseconds )
{
    libtocksync_alarm_delay_ms(milliseconds);
}
