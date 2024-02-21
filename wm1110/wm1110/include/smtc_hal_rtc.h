
#ifndef __SMTC_HAL_RTC
#define __SMTC_HAL_RTC

#include <stdint.h>
#include <stdbool.h>

#define RTC_2_PER_TICK	0.091552734375
#define RTC_2_MAX_TICKS	0xffffff

#ifdef __cplusplus
extern "C" {
#endif

void hal_rtc_init( void );

uint32_t hal_rtc_get_time_s( void );
uint32_t hal_rtc_get_time_ms( void );
uint32_t hal_rtc_get_time_100us( void );
uint32_t hal_rtc_get_max_ticks( void );

void hal_rtc_wakeup_timer_set_ms( const int32_t milliseconds );
void hal_rtc_wakeup_timer_stop( void );

#ifdef __cplusplus
}
#endif

#endif
