
#include <stdint.h>   // C99 types
#include <stdbool.h>  // bool type
#include <string.h>
// #include "smtc_hal_uart.h"
#include "smtc_hal_trace.h"

#define PRINT_BUFFER_SIZE 256

void hal_trace_print_var( const char* fmt, ... )
{
    va_list args;
    va_start( args, fmt );
    hal_trace_print( fmt, args );
    va_end( args );
}

char string[PRINT_BUFFER_SIZE];
void hal_trace_print( const char* fmt, va_list argp )
{
    // if( 0 < vsprintf( string, fmt, argp ) )  // build string
    // {
    //     hal_debug_init( );
    //     hal_uart_tx(( uint8_t* ) string, strlen( string ));
    //     hal_debug_deinit( );
    // }
    vprintf(fmt, argp); // directly prints formatted string
}