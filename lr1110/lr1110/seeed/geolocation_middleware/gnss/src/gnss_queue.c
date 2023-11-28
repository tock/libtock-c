/**
 * @file      gnss_queue.c
 *
 * @brief     Implementation of the GNSS scan group queue.
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2022. All rights reserved.
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

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stddef.h>
#include <string.h>

#include "mw_dbg_trace.h"
#include "gnss_queue.h"
#include "smtc_modem_hal.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

#define GNSS_QUEUE_FEATURE_OFF 0
#define GNSS_QUEUE_FEATURE_ON !GNSS_QUEUE_FEATURE_OFF

#ifndef GNSS_QUEUE_DBG_TRACE
#define GNSS_QUEUE_DBG_TRACE GNSS_QUEUE_FEATURE_OFF /* Enable/Disable traces here */
#endif

#if( GNSS_QUEUE_DBG_TRACE == GNSS_QUEUE_FEATURE_ON )
#define GNSS_QUEUE_TRACE_MSG( msg ) \
    do                              \
    {                               \
        MW_DBG_TRACE_PRINTF( msg ); \
    } while( 0 )

#define GNSS_QUEUE_TRACE_PRINTF( ... )      \
    do                                      \
    {                                       \
        MW_DBG_TRACE_PRINTF( __VA_ARGS__ ); \
    } while( 0 )

#define GNSS_QUEUE_PRINT( queue )                                                                     \
    {                                                                                                 \
        GNSS_QUEUE_TRACE_PRINTF( "****************************************\n" );                      \
        GNSS_QUEUE_TRACE_PRINTF( "token:       0x%02X\n", queue->token );                             \
        GNSS_QUEUE_TRACE_PRINTF( "group_size:  %d\n", queue->scan_group_size );                       \
        GNSS_QUEUE_TRACE_PRINTF( "scan_valid:  %d\n", queue->nb_scans_valid );                        \
        GNSS_QUEUE_TRACE_PRINTF( "scan_total:  %d\n", queue->nb_scans_total );                        \
        GNSS_QUEUE_TRACE_PRINTF( "next_send_index:   %d\n", queue->next_send_index );                 \
        GNSS_QUEUE_TRACE_PRINTF( "power_cons:  %d uah\n", queue->power_consumption_uah );             \
        for( uint8_t i = 0; i < queue->nb_scans_valid; i++ )                                          \
        {                                                                                             \
            GNSS_QUEUE_TRACE_PRINTF( "scans[%d]: %02d %02d %02d - ", i, queue->scans[i].detected_svs, \
                                     queue->scans[i].results_size, queue->scans[i].nav_valid );       \
            for( uint8_t j = 0; j < ( GNSS_SCAN_METADATA_SIZE + queue->scans[i].results_size ); j++ ) \
            {                                                                                         \
                GNSS_QUEUE_TRACE_PRINTF( "%02X ", queue->scans[i].results_buffer[j] );                \
            }                                                                                         \
            GNSS_QUEUE_TRACE_PRINTF( "\n" );                                                          \
        }                                                                                             \
        GNSS_QUEUE_TRACE_PRINTF( "****************************************\n" );                      \
    }

#else
#define GNSS_QUEUE_TRACE_MSG( msg )
#define GNSS_QUEUE_TRACE_PRINTF( ... )
#define GNSS_QUEUE_PRINT( queue )
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

void gnss_scan_group_queue_reset_token( gnss_scan_group_queue_t* queue )
{
    if( queue != NULL )
    {
        queue->token =
            ( uint8_t ) smtc_modem_hal_get_random_nb_in_range( 2, 0x1F ); /* 5-bits token with 0x00 and 0x01 excluded */
    }
}

void gnss_scan_group_queue_increment_token( gnss_scan_group_queue_t* queue )
{
    if( queue != NULL )
    {
        queue->token = ( queue->token + 1 ) % 0x20; /* roll-over on 5-bits */

        /* Exclude 0x00 and 0x01 values */
        if( queue->token == 0 )
        {
            queue->token = 2;
        }
    }
}

bool gnss_scan_group_queue_new( gnss_scan_group_queue_t* queue, uint8_t scan_group_size, uint8_t scan_group_stop )
{
    if( ( queue != NULL ) && ( scan_group_size <= GNSS_SCAN_GROUP_SIZE_MAX ) )
    {
        /* queue params */
        queue->scan_group_size = scan_group_size;
        queue->scan_group_stop = scan_group_stop;

        /* reset queue current status */
        queue->nb_scans_valid        = 0;
        queue->nb_scans_total        = 0;
        queue->next_send_index       = 0;
        queue->power_consumption_uah = 0;
        queue->stop                  = false;

        /* reset queue buffers */
        memset( queue->scans, 0, sizeof queue->scans );

        GNSS_QUEUE_TRACE_PRINTF( "%s:\n", __FUNCTION__ );
        GNSS_QUEUE_PRINT( queue );

        return true;
    }

    return false;
}

bool gnss_scan_group_queue_is_full( gnss_scan_group_queue_t* queue )
{
    if( queue != NULL )
    {
        /* the number of scan done reached group size */
        return ( ( queue->nb_scans_total == queue->scan_group_size ) || ( queue->stop == true ) );
    }

    return false;
}

bool gnss_scan_group_queue_is_valid( gnss_scan_group_queue_t* queue )
{
    if( queue != NULL )
    {
        if( ( ( queue->nb_scans_valid == 1 ) && ( queue->scans[0].nav_valid == true ) ) ||
            ( queue->nb_scans_valid > 1 ) )
        {
            return true;
        }
    }

    return false;
}

void gnss_scan_group_queue_push( gnss_scan_group_queue_t* queue, gnss_scan_t* scan )
{
    if( ( queue != NULL ) && ( scan != NULL ) )
    {
        /* Add the scan to the queue if valid */
        if( scan->detected_svs > 0 )
        {
            memcpy( &( queue->scans[queue->nb_scans_valid] ), scan, sizeof( gnss_scan_t ) );
            queue->nb_scans_valid += 1;
        }

        /* If a stop limit has been set and the current scan has enough detected SVs, the current scan group can be
         * stopped and sent */
        if( ( queue->scan_group_stop != 0 ) && ( scan->detected_svs >= queue->scan_group_stop ) )
        {
            queue->stop = true;
        }

        queue->nb_scans_total += 1;

        GNSS_QUEUE_TRACE_PRINTF( "%s:\n", __FUNCTION__ );
        GNSS_QUEUE_PRINT( queue );
    }
}

bool gnss_scan_group_queue_pop( gnss_scan_group_queue_t* queue, uint8_t** buffer, uint8_t* buffer_size )
{
    if( ( queue != NULL ) && gnss_scan_group_queue_is_valid( queue ) &&
        ( queue->next_send_index < queue->nb_scans_valid ) )
    {
        const uint8_t index   = queue->next_send_index;
        const uint8_t is_last = ( queue->next_send_index == ( queue->nb_scans_valid - 1 ) );

        /* Set scan group metadata
            | last NAV (1bit) | RFU (2bits) | token (5bits) |
            - token: scan group identifier
            - last NAV: indicates if this is the last NAV message of a scan group
        */
        queue->scans[index].results_buffer[0] = 0;
        queue->scans[index].results_buffer[0] = ( is_last << 7 ) | ( queue->token & 0x1F );

        /* Update queue info */
        queue->next_send_index += 1;

        /* Return a pointer to the buffer to be sent, and its size */
        *buffer      = queue->scans[index].results_buffer;
        *buffer_size = GNSS_SCAN_METADATA_SIZE + queue->scans[index].results_size;

        GNSS_QUEUE_TRACE_PRINTF( "%s:\n", __FUNCTION__ );
        GNSS_QUEUE_PRINT( queue );

        return true;
    }

    /* scan results to be sent */
    *buffer      = NULL;
    *buffer_size = 0;
    GNSS_QUEUE_TRACE_PRINTF( "%s: no scan result left in queue\n", __FUNCTION__ );

    return false;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/* --- EOF ------------------------------------------------------------------ */