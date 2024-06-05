
#if 0

#include "smtc_hal_dbg_trace.h"
#include "nrf_drv_timer.h"
#include "smtc_hal_lp_time.h"

static const nrf_drv_timer_t TIMER_LP = NRF_DRV_TIMER_INSTANCE( 2 );
static hal_lp_timer_irq_t lptim_tmr_irq = { .context = NULL, .callback = NULL };

static bool lp_timer_init = false;

static void timer_event_handler( nrf_timer_event_t event_type, void* p_context)
{
    hal_lp_timer_stop( );
    hal_lp_timer_deinit( );
    if( lptim_tmr_irq.callback != NULL )
    {
        lptim_tmr_irq.callback( lptim_tmr_irq.context );
    }
}

void hal_lp_timer_init( void )
{
    if( lp_timer_init == false )
    {
        lp_timer_init = true;
        nrf_drv_clock_hfclk_request( NULL );
        nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
        nrf_drv_timer_init( &TIMER_LP, &timer_cfg, timer_event_handler );
    }
}

void hal_lp_timer_deinit( void )
{
    if( lp_timer_init == true )
    {
        lp_timer_init = false;
        nrf_drv_clock_hfclk_release( );
        nrf_drv_timer_uninit( &TIMER_LP );
    }
}

void hal_lp_timer_start( const uint32_t milliseconds, const hal_lp_timer_irq_t* tmr_irq )
{
    uint32_t time_ticks;

    hal_lp_timer_init( );

    time_ticks = nrf_drv_timer_ms_to_ticks( &TIMER_LP, milliseconds );
    nrf_drv_timer_extended_compare( &TIMER_LP, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true );

    lptim_tmr_irq = *tmr_irq;

    nrf_drv_timer_enable( &TIMER_LP );
}

void hal_lp_timer_stop( void )
{
    if( lp_timer_init == true )
    {
        nrf_drv_timer_disable( &TIMER_LP );
    }
}

void hal_lp_timer_irq_enable( void )
{
    if( lp_timer_init == true )
    {
        nrfx_timer_compare_int_enable( &TIMER_LP, NRF_TIMER_CC_CHANNEL0 );
    }
}

void hal_lp_timer_irq_disable( void )
{
    if( lp_timer_init == true )
    {
        nrfx_timer_compare_int_disable( &TIMER_LP, NRF_TIMER_CC_CHANNEL0 );
    }
}

#endif

#if 1

#include "smtc_hal_dbg_trace.h"
//#include "nrf_drv_timer.h"
//#include "nrf_drv_rtc.h"
//#include "nrf_drv_clock.h"
#include "smtc_hal_lp_time.h"
#include <libtock-sync/services/alarm.h>
#include <libtock/peripherals/syscalls/alarm_syscalls.h>

#define RTC_1_MAX_TICKS	0xffffff
#define RTC_1_PER_TICK	1.007080078125

// static const nrf_drv_rtc_t rtc_1 = NRF_DRV_RTC_INSTANCE( 1 );
// static nrf_drv_rtc_config_t rtc_config_1 = NRF_DRV_RTC_DEFAULT_CONFIG;
static hal_lp_timer_irq_t lptim_tmr_irq = { .context = NULL, .callback = NULL };

// static bool lp_timer_init = false;
// static uint32_t lp_timer_old_slot = 0;

// static void timer_event_handler( nrf_timer_event_t event_type, void* p_context)
// {
//     // hal_lp_timer_stop( );
//     // hal_lp_timer_deinit( );
//     // if( lptim_tmr_irq.callback != NULL )
//     // {
//     //     lptim_tmr_irq.callback( lptim_tmr_irq.context );
//     // }
// }

static void timer_tock_upcall(__attribute__ ((unused)) int unused0,
                         __attribute__ ((unused)) int unused1,
                         __attribute__ ((unused)) int unused2,
                         __attribute__ ((unused)) void* ud) {
    printf("ttu\n");
    if( lptim_tmr_irq.callback != NULL )
    {
        lptim_tmr_irq.callback( lptim_tmr_irq.context );
    }

}

// void hal_lp_timer_init( void )
// {
//     if( lp_timer_init == false )
//     {
//         lp_timer_init = true;
//         rtc_config_1.prescaler = 32; // base tick is 1.007080078125 ms
//         nrf_drv_rtc_init( &rtc_1, &rtc_config_1, timer_event_handler );
//     }
// }

// void hal_lp_timer_deinit( void )
// {
//     if( lp_timer_init == true )
//     {
//         lp_timer_init = false;
//         nrf_drv_rtc_uninit( &rtc_1 );
//     }
// }

void hal_lp_timer_start( const uint32_t milliseconds, const hal_lp_timer_irq_t* tmr_irq )
{
    // // hal_lp_timer_init( );
    if( milliseconds > 1 )
    {
    //     uint32_t ticks = 0;
    //     uint32_t frequency = 0;

    //     alarm_internal_frequency(&frequency);
    //     ticks = (uint32_t)(((float)milliseconds / 1000) * frequency);

    //     uint32_t current_time = 0;
    //     alarm_internal_read(&current_time);
    //     alarm_internal_set(current_time, ticks);

    //     // uint32_t time_ticks = 0;
    //     // float time_ticks_f = milliseconds;
    //     // time_ticks_f = time_ticks_f / RTC_1_PER_TICK + 0.5;
    //     // // time_ticks = nrf_drv_rtc_counter_get( &rtc_1 ) + time_ticks_f;
    //     // time_ticks = time_ticks_f;
    //     // nrf_drv_rtc_counter_clear( &rtc_1 );
    //     // nrf_drv_rtc_cc_set( &rtc_1, 0, time_ticks & RTC_1_MAX_TICKS, true );

    //     // nrf_drv_rtc_enable( &rtc_1 );


        // lptim_tmr_irq = *tmr_irq;
        //   bool cond = false;
        //   tock_timer_t timer;

        //   timer_in(milliseconds, timer_tock_upcall, &cond, &timer);

          libtocksync_alarm_delay_ms(milliseconds);
          tmr_irq->callback(tmr_irq->context);

    }
    else // execute immediately
    {
    //     if (tmr_irq && tmr_irq->callback) {
            tmr_irq->callback(tmr_irq->context);
    //     }

    //     // if( lptim_tmr_irq.callback != NULL )
    //     // {
            // lptim_tmr_irq.callback( lptim_tmr_irq.context );
    //     // }
    }
}

void hal_lp_timer_stop( void )
{
    libtock_alarm_command_stop();

    // if( lp_timer_init == true )
    // {
    //     nrf_drv_rtc_disable( &rtc_1 );
    // }
}

void hal_lp_timer_irq_enable( void )
{

}

void hal_lp_timer_irq_disable( void )
{

}

#endif
