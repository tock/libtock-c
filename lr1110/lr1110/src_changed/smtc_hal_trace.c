
#include <stdint.h>   // C99 types
#include <stdbool.h>  // bool type
#include <string.h>

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
    vprintf(fmt, argp); // directly prints formatted string
}