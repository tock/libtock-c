
#ifndef __SMTC_HAL_LP_TIME__
#define __SMTC_HAL_LP_TIME__

#include <stdint.h>

typedef struct hal_lp_timer_irq_s
{
    void* context;
    void ( *callback )( void* context );
} hal_lp_timer_irq_t;

#ifdef __cplusplus
extern "C" {
#endif

void hal_lp_timer_init( void );

void hal_lp_timer_task_init( void );

void hal_lp_timer_deinit( void );

void hal_lp_timer_start( const uint32_t milliseconds, const hal_lp_timer_irq_t* tmr_irq );

void hal_lp_timer_stop( void );

void hal_lp_timer_irq_enable( void );

void hal_lp_timer_irq_disable( void );

#ifdef __cplusplus
}
#endif

#endif
