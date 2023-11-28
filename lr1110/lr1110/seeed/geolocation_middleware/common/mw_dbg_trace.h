/*!
 * @file      mw_dbg_trace.h
 *
 * @brief     Middleware debug trace definition.
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2021. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef MW_DBG_TRACE_H
#define MW_DBG_TRACE_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */
#include <stdint.h>   // C99 types
#include <stdbool.h>  // bool type

#include "smtc_modem_hal.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

#ifndef MW_DBG_TRACE
#define MW_DBG_TRACE 1
#endif

#ifndef MW_DBG_TRACE_COLOR
#define MW_DBG_TRACE_COLOR 1
#endif

#if( MW_DBG_TRACE_COLOR == 1 )
#define MW_DBG_TRACE_COLOR_RED "\x1B[0;31m"
#define MW_DBG_TRACE_COLOR_GREEN "\x1B[0;32m"
#define MW_DBG_TRACE_COLOR_YELLOW "\x1B[0;33m"
#define MW_DBG_TRACE_COLOR_DEFAULT "\x1B[0m"
#else
#define MW_DBG_TRACE_COLOR_RED ""
#define MW_DBG_TRACE_COLOR_GREEN ""
#define MW_DBG_TRACE_COLOR_YELLOW ""
#define MW_DBG_TRACE_COLOR_DEFAULT ""
#endif

#if( MW_DBG_TRACE == 1 )

#define MW_DBG_TRACE_PRINTF( ... ) smtc_modem_hal_print_trace( __VA_ARGS__ )

#define MW_DBG_TRACE_MSG( msg )                            \
    do                                                     \
    {                                                      \
        MW_DBG_TRACE_PRINTF( MW_DBG_TRACE_COLOR_DEFAULT ); \
        MW_DBG_TRACE_PRINTF( msg );                        \
    } while( 0 )

#define MW_DBG_TRACE_INFO( ... )                           \
    do                                                     \
    {                                                      \
        MW_DBG_TRACE_PRINTF( MW_DBG_TRACE_COLOR_GREEN );   \
        MW_DBG_TRACE_PRINTF( "INFO: " );                   \
        MW_DBG_TRACE_PRINTF( __VA_ARGS__ );                \
        MW_DBG_TRACE_PRINTF( MW_DBG_TRACE_COLOR_DEFAULT ); \
    } while( 0 )

#define MW_DBG_TRACE_WARNING( ... )                        \
    do                                                     \
    {                                                      \
        MW_DBG_TRACE_PRINTF( MW_DBG_TRACE_COLOR_YELLOW );  \
        MW_DBG_TRACE_PRINTF( "WARN: " );                   \
        MW_DBG_TRACE_PRINTF( __VA_ARGS__ );                \
        MW_DBG_TRACE_PRINTF( MW_DBG_TRACE_COLOR_DEFAULT ); \
    } while( 0 )

#define MW_DBG_TRACE_ERROR( ... )                          \
    do                                                     \
    {                                                      \
        MW_DBG_TRACE_PRINTF( MW_DBG_TRACE_COLOR_RED );     \
        MW_DBG_TRACE_PRINTF( "ERROR: " );                  \
        MW_DBG_TRACE_PRINTF( __VA_ARGS__ );                \
        MW_DBG_TRACE_PRINTF( MW_DBG_TRACE_COLOR_DEFAULT ); \
    } while( 0 )

#define MW_DBG_TRACE_ARRAY( msg, array, len )                                \
    do                                                                       \
    {                                                                        \
        MW_DBG_TRACE_PRINTF( "%s - (%lu bytes):\n", msg, ( uint32_t ) len ); \
        for( uint32_t i = 0; i < ( uint32_t ) len; i++ )                     \
        {                                                                    \
            if( ( ( i % 16 ) == 0 ) && ( i > 0 ) )                           \
            {                                                                \
                MW_DBG_TRACE_PRINTF( "\n" );                                 \
            }                                                                \
            MW_DBG_TRACE_PRINTF( " %02X", array[i] );                        \
        }                                                                    \
        MW_DBG_TRACE_PRINTF( "\n" );                                         \
    } while( 0 )

#else
#define MW_DBG_TRACE_PRINTF( ... )
#define MW_DBG_TRACE_MSG( msg )
#define MW_DBG_TRACE_INFO( ... )
#define MW_DBG_TRACE_WARNING( ... )
#define MW_DBG_TRACE_ERROR( ... )
#define MW_DBG_TRACE_ARRAY( msg, array, len )
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

#ifdef __cplusplus
}
#endif

#endif  // MW_DBG_TRACE_H

/* --- EOF ------------------------------------------------------------------ */
