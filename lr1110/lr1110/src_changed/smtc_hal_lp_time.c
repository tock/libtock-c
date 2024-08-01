#include "smtc_hal_dbg_trace.h"
#include "smtc_hal_lp_time.h"

#include <libtock-sync/services/alarm.h>
#include <libtock/peripherals/syscalls/alarm_syscalls.h>

static hal_lp_timer_irq_t lptim_tmr_irq = { .context = NULL, .callback = NULL };


static void timer_tock_upcall(__attribute__ ((unused)) int unused0,
                         __attribute__ ((unused)) int unused1,
                         __attribute__ ((unused)) int unused2,
                         __attribute__ ((unused)) void* ud) {
    if( lptim_tmr_irq.callback != NULL )
    {
        lptim_tmr_irq.callback( lptim_tmr_irq.context );
    }

}

void hal_lp_timer_start( const uint32_t milliseconds, const hal_lp_timer_irq_t* tmr_irq )
{
    if( milliseconds > 1 )
    {
          libtocksync_alarm_delay_ms(milliseconds);
          tmr_irq->callback(tmr_irq->context);
    }
    else // execute immediately
    {
            tmr_irq->callback(tmr_irq->context);
    }
}

void hal_lp_timer_stop( void )
{
    libtock_alarm_command_stop();
}

void hal_lp_timer_irq_enable( void )
{

}

void hal_lp_timer_irq_disable( void )
{

}
