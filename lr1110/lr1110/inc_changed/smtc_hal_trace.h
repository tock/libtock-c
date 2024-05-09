
#ifndef __SMTC_HAL_TRACE_H
#define __SMTC_HAL_TRACE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void hal_trace_print( const char* fmt, va_list argp );
void hal_trace_print_var( const char* fmt, ... );

#endif

#ifdef __cplusplus
}
#endif