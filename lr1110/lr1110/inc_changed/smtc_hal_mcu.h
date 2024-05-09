
#ifndef __SMTC_HAL_MCU_H
#define __SMTC_HAL_MCU_H

#include <stdbool.h>
#include <stdint.h>

#include "smtc_hal.h"

/**
 * @brief Panic function for mcu issues
 */
#ifdef __cplusplus
extern "C" {
#endif

#define TRACE_PRINTF( ... ) hal_trace_print_var( __VA_ARGS__ )

/*!
 * Panic function for mcu issues
 */
#define mcu_panic( ... )                            \
    do                                              \
    {                                               \
        TRACE_PRINTF( "mcu_panic:%s\n", __func__ ); \
        TRACE_PRINTF( "-> "__VA_ARGS__ );           \
        hal_mcu_reset( );                           \
    } while( 0 );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */
void hal_mcu_init( void );

void hal_mcu_disable_irq( void );

void hal_mcu_enable_irq( void );

void hal_clock_init(void);

void hal_pwr_init(void);

void hal_mcu_reset( void );

void hal_mcu_wait_us( const int32_t microseconds );

void hal_mcu_wait_ms( const int32_t ms );

void hal_mcu_partial_sleep_enable( bool enable );

void hal_mcu_set_sleep_for_ms( const int32_t milliseconds );

void hal_mcu_trace_print( const char* fmt, ... );

void hal_hex_to_bin( char *input, uint8_t *dst, int len );

void hal_print_bin_to_hex( uint8_t *buf, uint16_t len );

#endif

#ifdef __cplusplus
}
#endif
