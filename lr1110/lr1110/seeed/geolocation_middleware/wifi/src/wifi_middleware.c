/**
 * @file      wifi_middleware.c
 *
 * @brief     Wi-Fi geolocation middleware implementing scan & send sequence.
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

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>

#include "wifi_version.h"
#include "wifi_middleware.h"
#include "wifi_helpers.h"

#include "mw_assert.h"
#include "mw_dbg_trace.h"

#include "lr11xx_system.h"

#include "smtc_modem_middleware_advanced_api.h"
#include "smtc_modem_hal.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

#define WIFI_MW_FEATURE_OFF 0
#define WIFI_MW_FEATURE_ON !WIFI_MW_FEATURE_OFF

#ifndef WIFI_MW_DBG_TRACE_TIME_CRITICAL
#define WIFI_MW_DBG_TRACE_TIME_CRITICAL WIFI_MW_FEATURE_OFF /* Enable/Disable traces here */
#endif

#if( WIFI_MW_DBG_TRACE_TIME_CRITICAL == WIFI_MW_FEATURE_ON )
#define WIFI_MW_TIME_CRITICAL_TRACE_MSG( msg ) \
    do                                         \
    {                                          \
        MW_DBG_TRACE_PRINTF( msg );            \
    } while( 0 )

#define WIFI_MW_TIME_CRITICAL_TRACE_PRINTF( ... ) \
    do                                            \
    {                                             \
        MW_DBG_TRACE_PRINTF( __VA_ARGS__ );       \
    } while( 0 )

#else
#define WIFI_MW_TIME_CRITICAL_TRACE_MSG( msg )
#define WIFI_MW_TIME_CRITICAL_TRACE_PRINTF( ... )
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/**
 * @brief Radio planner task ID for WiFi middleware
 */
#define RP_TASK_WIFI SMTC_MODEM_RP_TASK_ID2

/**
 * @brief LoRaWAN port used for uplinks of the WIFI scan results
 */
#define WIFI_DEFAULT_UPLINK_PORT 197

/**
 * @brief Minimal number of detected access point in a scan result to consider the scan valid
 */
#define WIFI_SCAN_NB_AP_MIN ( 3 )

/**
 * @brief Size in bytes of a WiFi Access-Point address
 */
#define WIFI_AP_ADDRESS_SIZE ( 6 )

/**
 * @brief Size in bytes to store the RSSI of a detected WiFi Access-Point
 */
#define WIFI_AP_RSSI_SIZE ( 1 )

/**
 * @brief Size in bytes of the payload tag to indicate frame format (as defined by LR1110 WiFi positioning protocol of
 * LoRaCloud)
 */
#define WIFI_TAG_SIZE ( 1 )

/**
 * @brief The LoRa Basics Modem extended uplink ID to be used for Wi-Fi uplinks (TASK_EXTENDED_2)
 */
#define SMTC_MODEM_EXTENDED_UPLINK_ID_WIFI ( 2 )
/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/**
 * @brief The list of possible internal pending errors
 */
typedef enum
{
    WIFI_MW_ERROR_NONE,         //!< No error
    WIFI_MW_ERROR_SCAN_FAILED,  //!< Scan failed due to LR11xx error
    WIFI_MW_ERROR_UNKNOWN,      //!< An unknown error occurred
} wifi_mw_internal_error_t;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/**
 * @brief The modem/radio context given by the application when middleware is initialized. Used to access LBM and radio
 * functions.
 */
static ralf_t* modem_radio_ctx = NULL;

/**
 * @brief The modem stack ID to be used.
 */
static uint8_t modem_stack_id = 0;

/*!
 * @brief The current pending errors (reset when a new scan sequence starts)
 */
static wifi_mw_internal_error_t pending_error = WIFI_MW_ERROR_NONE;

/*!
 * @brief The current pending events (reset by the user or when a new scan sequence starts)
 */
static uint8_t pending_events = 0;

/*!
 * @brief Results of the current Wi-Fi scan
 */
static wifi_scan_all_result_t wifi_results;

/*!
 * @brief The buffer containing results to be sent over the air
 */
static uint8_t wifi_result_buffer[WIFI_TAG_SIZE + ( ( WIFI_AP_RSSI_SIZE + WIFI_AP_ADDRESS_SIZE ) * WIFI_MAX_RESULTS )];

/*!
 * @brief User has requested to cancel the scan that was scheduled
 */
static bool task_cancelled_by_user = false;

/*!
 * @brief The scan sequence has started
 *
 * Set to true when the first scan of the sequence actually started (radio)
 * Set back to false when the complete sequence is terminated (all results sent)
 */
static bool task_running = false;

/*!
 * @brief Indicates sequence to "scan & send" or "scan only" mode
 */
static bool send_bypass = false;

/*!
 * @brief The LoRaWAN port on which WiFi scan results are sent
 */
static uint8_t lorawan_port = WIFI_DEFAULT_UPLINK_PORT;

/*!
 * @brief The format of the Wi-Fi scan results to be used.
 */
static wifi_mw_payload_format_t payload_format = WIFI_MW_PAYLOAD_MAC;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*!
 * @brief Interrupt handler signaled by the Radio Planner when the radio is available and it is time to start the scan
 * (WARNING: running under interrupt context)
 *
 * @param [in] context Pointer to context given by RP (not used)
 */
static void wifi_mw_scan_rp_task_launch( void* context );

/*!
 * @brief Interrupt handler signaled by the Radio Planner when the scan is completed (WARNING: running under interrupt
 * context)
 *
 * @param [in] status IRQ status from RP
 */
static void wifi_mw_scan_rp_task_done( smtc_modem_rp_status_t* status );

/*!
 * @brief Send the scan results over the air (uses extended API from LBM to send uplink)
 *
 * @return a boolean set to true if a frame has been sent, set to false is there is nothing to be sent (bypass mode,
 * invalid scan, send error...).
 */
static bool wifi_mw_send_results( void );

/*!
 * @brief Clear the results structure
 */
static void wifi_mw_reset_results( void );

/*!
 * @brief Callback called by the LBM when the uplink has been sent.
 */
static void wifi_mw_tx_done_callback( void );

/*!
 * @brief Send an uplink request to LBM through the extended API (no buffer copy)
 *
 * @param [in] tx_frame_buffer A pointer to the buffer payload to be sent over the air.
 * @param [in] tx_frame_buffer_size The size of the buffer to be sent.
 *
 * @return a boolean set to true for success, false otherwise.
 */
static bool wifi_mw_send_frame( const uint8_t* tx_frame_buffer, const uint8_t tx_frame_buffer_size );

/*!
 * @brief Add an event to the pending event bitfield, and send all pending events to the application
 *
 * @param [in] event_type The event to be added to the pending events bitfield.
 */
static void wifi_mw_send_event( wifi_mw_event_type_t event_type );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

mw_return_code_t wifi_mw_get_version( mw_version_t* version )
{
    if( version == NULL )
    {
        MW_DBG_TRACE_ERROR( "Failed to get Wi-Fi middleware version\n" );
        return MW_RC_FAILED;
    }

    version->major = WIFI_MW_VERSION_MAJOR;
    version->minor = WIFI_MW_VERSION_MINOR;
    version->patch = WIFI_MW_VERSION_PATCH;

    return MW_RC_OK;
}

mw_return_code_t wifi_mw_init( ralf_t* modem_radio, uint8_t stack_id )
{
    if( modem_radio == NULL )
    {
        MW_DBG_TRACE_ERROR( "Failed to init Wi-Fi middleware: modem radio not set\n" );
        return MW_RC_FAILED;
    }

    /* Set radio context */
    modem_radio_ctx = modem_radio;

    /* Set the stack id */
    modem_stack_id = stack_id;

    return MW_RC_OK;
}

mw_return_code_t wifi_mw_scan_start( uint32_t start_delay )
{
    smtc_modem_return_code_t modem_rc;
    smtc_modem_rp_task_t     rp_task = { 0 };
    uint32_t                 time_ms;
    wifi_settings_t          wifi_settings = { 0 };

    if( modem_radio_ctx == NULL )
    {
        MW_DBG_TRACE_ERROR( "Wi-Fi middleware not ready, cannot start scan\n" );
        return MW_RC_FAILED;
    }

    if( task_running == true )
    {
        MW_DBG_TRACE_ERROR( "Wi-Fi scan on-going. Cancel it before starting a new one\n" );
        return MW_RC_BUSY;
    }

    /* Reset pending internal error */
    pending_error = WIFI_MW_ERROR_NONE;

    /* Reset pending events */
    pending_events = 0;

    /* Reset any pending cancel request which has not been completed (error case) */
    task_cancelled_by_user = false;

    /* Init settings */
    wifi_settings.channels            = 0x3FFF; /* all channels enabled */
    wifi_settings.types               = LR11XX_WIFI_TYPE_SCAN_B_G_N;
    wifi_settings.max_results         = WIFI_MAX_RESULTS;
    wifi_settings.timeout_per_channel = WIFI_TIMEOUT_PER_CHANNEL_DEFAULT;
    wifi_settings.timeout_per_scan    = WIFI_TIMEOUT_PER_SCAN_DEFAULT;
    smtc_wifi_settings_init( &wifi_settings );

    /* Prepare the task for next scan, add 300ms to avoid schedule a task in the past */
    time_ms = smtc_modem_hal_get_time_in_ms( ) + 300;

    rp_task.type                 = SMTC_MODEM_RP_TASK_STATE_ASAP;
    rp_task.start_time_ms        = time_ms + ( start_delay * 1000 );
    rp_task.duration_time_ms     = 10 * 1000;
    rp_task.id                   = RP_TASK_WIFI;
    rp_task.launch_task_callback = wifi_mw_scan_rp_task_launch;
    rp_task.end_task_callback    = wifi_mw_scan_rp_task_done;
    modem_rc                     = smtc_modem_rp_add_user_radio_access_task( &rp_task );

    if( modem_rc == SMTC_MODEM_RC_OK )
    {
        MW_DBG_TRACE_INFO( "RP_TASK_WIFI - new scan - task queued at %u + %u\n", time_ms, start_delay * 1000 );
    }
    else
    {
        MW_DBG_TRACE_ERROR( "Failed to queue Wi-Fi scan task (0x%02X)\n", modem_rc );
        return MW_RC_FAILED;
    }

    return MW_RC_OK;
}

mw_return_code_t wifi_mw_scan_cancel( void )
{
    smtc_modem_return_code_t modem_rc;

    if( modem_radio_ctx == NULL )
    {
        MW_DBG_TRACE_ERROR( "Wi-Fi middleware not ready, no scan to cancel\n" );
        return MW_RC_FAILED;
    }

    /* The Wi-Fi scan sequence will be in running state from the moment the task
    has been started by the RP, until all the packets have been sent over the
    air. This is handled this way for more simplicity:
        - as we cannot abort a running scan, it would requires to check RP state
        to know if we can abort the scheduled task or not.
        - aborting anywhere in the complete sequence will require lot of logic
        through all the code, we want to keep as simple as possible
    So a scan can be cancelled only if requested before the first scan as
    actually started. Once it is started, it will complete the sequence */
    if( task_running == true )
    {
        MW_DBG_TRACE_ERROR( "Wi-Fi scan sequence started, too late to cancel\n" );
        return MW_RC_BUSY;
    }

    task_cancelled_by_user = true;

    MW_DBG_TRACE_INFO( "RP_TASK_WIFI - Request cancel of scheduled scan\n" );
    modem_rc = smtc_modem_rp_abort_user_radio_access_task( RP_TASK_WIFI );
    if( modem_rc != SMTC_MODEM_RC_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to abort Wi-Fi scan task\n" );
    }

    return MW_RC_OK;
}

bool wifi_mw_has_event( uint8_t pending_events, wifi_mw_event_type_t event )
{
    if( ( pending_events & ( 1 << event ) ) == ( 1 << event ) )
    {
        return true;
    }

    return false;
}

mw_return_code_t wifi_mw_get_event_data_scan_done( wifi_mw_event_data_scan_done_t* data )
{
    if( data == NULL )
    {
        MW_DBG_TRACE_ERROR( "Provided pointer is NULL\n" );
        return MW_RC_FAILED;
    }

    if( wifi_mw_has_event( pending_events, WIFI_MW_EVENT_SCAN_DONE ) )
    {
        data->nbr_results           = wifi_results.nbr_results;
        data->power_consumption_uah = wifi_results.power_consumption_uah;
        data->timestamp             = wifi_results.timestamp;
        for( uint8_t i = 0; i < wifi_results.nbr_results; i++ )
        {
            data->results[i].rssi    = wifi_results.results[i].rssi;
            data->results[i].channel = wifi_results.results[i].channel;
            data->results[i].type    = wifi_results.results[i].type;
            memcpy( data->results[i].mac_address, wifi_results.results[i].mac_address, WIFI_AP_ADDRESS_SIZE );
        }

        return MW_RC_OK;
    }
    else
    {
        MW_DBG_TRACE_ERROR( "Data are not ready\n" );
        return MW_RC_FAILED;
    }
}

void wifi_mw_set_port( uint8_t port ) { lorawan_port = port; }

void wifi_mw_send_bypass( bool no_send )
{
    MW_DBG_TRACE_INFO( "Wi-Fi scan: set scan only mode to %s (bypass send)\n", no_send ? "TRUE" : "FALSE" );

    /* Set scan only current mode */
    send_bypass = no_send;
}

void wifi_mw_set_payload_format( wifi_mw_payload_format_t format ) { payload_format = format; }

void wifi_mw_display_results( const wifi_mw_event_data_scan_done_t* data )
{
    if( data != NULL )
    {
        MW_DBG_TRACE_PRINTF( "SCAN_DONE info:\n" );
        MW_DBG_TRACE_PRINTF( "-- number of results: %u\n", data->nbr_results );
        MW_DBG_TRACE_PRINTF( "-- power consumption: %u uah\n", data->power_consumption_uah );
        MW_DBG_TRACE_PRINTF( "-- timestamp: %u\n", data->timestamp );

        for( uint8_t i = 0; i < data->nbr_results; i++ )
        {
            for( uint8_t j = 0; j < WIFI_AP_ADDRESS_SIZE; j++ )
            {
                MW_DBG_TRACE_PRINTF( "%02X ", data->results[i].mac_address[j] );
            }
            MW_DBG_TRACE_PRINTF( " -- Channel: %d", data->results[i].channel );
            MW_DBG_TRACE_PRINTF( " -- Type: %d", data->results[i].type );
            MW_DBG_TRACE_PRINTF( " -- RSSI: %d\n", data->results[i].rssi );
        }
    }
}

mw_return_code_t wifi_mw_get_event_data_terminated( wifi_mw_event_data_terminated_t* data )
{
    if( data == NULL )
    {
        MW_DBG_TRACE_ERROR( "Provided pointer is NULL\n" );
        return MW_RC_FAILED;
    }

    if( wifi_mw_has_event( pending_events, WIFI_MW_EVENT_TERMINATED ) )
    {
        /* Result are sent only if enough results */
        if( send_bypass == false )
        {
            data->nb_scans_sent = ( wifi_results.nbr_results >= WIFI_SCAN_NB_AP_MIN ) ? 1 : 0;
        }
        else
        {
            /* assume that the "no send" mode was configured before starting the scan, so no packet sent */
            data->nb_scans_sent = 0;
        }

        return MW_RC_OK;
    }
    else
    {
        MW_DBG_TRACE_ERROR( "Scan is not terminated\n" );
        return MW_RC_FAILED;
    }
}

void wifi_mw_clear_pending_events( void ) { pending_events = 0; }

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

static void wifi_mw_scan_rp_task_launch( void* context )
{
    MW_DBG_TRACE_MSG( "---- internal Wi-Fi scan start ----\n" );

    /* From now, the scan sequence can not be cancelled */
    task_running = true;

    /* Reset previous results */
    wifi_mw_reset_results( );

    /* Start WIFI scan */
    if( smtc_wifi_start_scan( modem_radio_ctx->ral.context ) != true )
    {
        pending_error = WIFI_MW_ERROR_SCAN_FAILED;

        MW_DBG_TRACE_ERROR( "RP_TASK_WIFI - failed to start scan, abort task\n" );
        MW_ASSERT_SMTC_MODEM_RC( smtc_modem_rp_abort_user_radio_access_task( RP_TASK_WIFI ) );

        /*
        When aborting the task, the RP will call the end_task_callback() with SMTC_RP_RADIO_ABORTED status.
        ERROR event will be sent from there to the application
        */
    }
}

void wifi_mw_scan_rp_task_done( smtc_modem_rp_status_t* status )
{
    uint32_t                     time_ms;
    smtc_modem_rp_radio_status_t irq_status = status->status;
    uint32_t                     meas_time;

    /* ----------------------------------------------------------------- */
    /* WARNING: put the radio back to sleep before exiting this function */
    /* ----------------------------------------------------------------- */

    /* Save current time to restart task */
    time_ms = smtc_modem_hal_get_time_in_ms( );
    WIFI_MW_TIME_CRITICAL_TRACE_PRINTF( "WIFI scan done at %d (irq_status=%d)\n", time_ms, irq_status );

    /* WIFI scan completed or aborted - first thing to be done */
    smtc_wifi_scan_ended( );

    if( irq_status == SMTC_RP_RADIO_ABORTED )
    {
        if( pending_error == WIFI_MW_ERROR_NONE )
        {
            if( task_cancelled_by_user == false )
            {
                MW_DBG_TRACE_WARNING( "RP_TASK_WIFI(%d) - task aborted by RP\n", __LINE__ );

                /* Notify the application that the operation has been terminated */
                wifi_mw_send_event( WIFI_MW_EVENT_SCAN_DONE );
                wifi_mw_send_event( WIFI_MW_EVENT_TERMINATED );
            }
            else
            {
                MW_DBG_TRACE_WARNING( "RP_TASK_WIFI(%d) - task cancelled by user\n", __LINE__ );

                /* We handle here the fact that when the user requests to abort a radio planner task
                with smtc_modem_rp_abort_user_radio_access_task(), the RP will call this task_done callback
                with status set to SMTC_RP_RADIO_ABORTED */

                /* reset cancel request status */
                task_cancelled_by_user = false;

                /* Send an event to application to notify for error */
                wifi_mw_send_event( WIFI_MW_EVENT_SCAN_CANCELLED );
            }
        }
        else
        {
            MW_DBG_TRACE_WARNING( "RP_TASK_WIFI(%d) - task aborted for UNKNOWN reason\n", __LINE__ );

            /* Send an event to application to notify for error */
            wifi_mw_send_event( WIFI_MW_EVENT_ERROR_UNKNOWN );
        }
    }
    else if( irq_status == SMTC_RP_RADIO_WIFI_SCAN_DONE )
    {
        bool scan_results_rc;

        memset( &wifi_results, 0, sizeof wifi_results );

        /* Timestamp this scan */
        wifi_results.timestamp = mw_get_gps_time( );

        /* Wi-Fi scan completed, get and display the results */
        scan_results_rc = smtc_wifi_get_results( modem_radio_ctx->ral.context, &wifi_results );

        /* Get scan power consumption */
        smtc_wifi_get_power_consumption( modem_radio_ctx->ral.context, &wifi_results.power_consumption_uah );

        if( scan_results_rc == true )
        {
            /* Scan has been completed, send an event to application */
            wifi_mw_send_event( WIFI_MW_EVENT_SCAN_DONE );

            /* Send scan uplink if any, or send event to application */
            if( wifi_mw_send_results( ) == false )
            {
                /* No more scan result to be sent, or failed to send packet */
                WIFI_MW_TIME_CRITICAL_TRACE_PRINTF( "RP_TASK_WIFI: no scan result to be sent\n" );

                /* Send an event to application to notify for completion */
                /* The application needs to know that it can proceed with the next scan */
                wifi_mw_send_event( WIFI_MW_EVENT_TERMINATED );
            }
        }
        else
        {
            MW_DBG_TRACE_ERROR( "RP_TASK_WIFI - unkown error on get results\n" );
            /* Send an event to application to notify for error */
            wifi_mw_send_event( WIFI_MW_EVENT_ERROR_UNKNOWN );
        }
    }
    else
    {
        MW_DBG_TRACE_ERROR( "WIFI RP task - Unknown status %d at %d\n", irq_status, time_ms );

        /* Send an event to application to notify for error */
        wifi_mw_send_event( WIFI_MW_EVENT_ERROR_UNKNOWN );
    }

    /* Monitor callback exec duration (should be kept as low as possible) */
    meas_time = smtc_modem_hal_get_time_in_ms( );
    MW_DBG_TRACE_WARNING( "WIFI RP task - done callback duration %u ms\n", meas_time - time_ms );

    /* Set the radio back to sleep */
    mw_radio_set_sleep( modem_radio_ctx->ral.context );
}

/*!
 * @brief User private function
 */

static bool wifi_mw_send_results( void )
{
    uint8_t wifi_buffer_size = 0;

    /* Check if "no send "mode" is configured */
    if( send_bypass == true )
    {
        /* Bypass send */
        return false;
    }

    /* Check if there are results to be sent */
    if( wifi_results.nbr_results < WIFI_SCAN_NB_AP_MIN )
    {
        return false;
    }

    /* Add the payload format tag */
    wifi_result_buffer[wifi_buffer_size] = payload_format;
    wifi_buffer_size += WIFI_TAG_SIZE;

    /* Concatenate all results in send buffer */
    for( uint8_t i = 0; i < wifi_results.nbr_results; i++ )
    {
        /* Copy Access Point RSSI address in result buffer (if requested) */
        if( payload_format == WIFI_MW_PAYLOAD_MAC_RSSI )
        {
            wifi_result_buffer[wifi_buffer_size] = wifi_results.results[i].rssi;
            wifi_buffer_size += WIFI_AP_RSSI_SIZE;
        }
        /* Copy Access Point MAC address in result buffer */
        memcpy( &wifi_result_buffer[wifi_buffer_size], wifi_results.results[i].mac_address, WIFI_AP_ADDRESS_SIZE );
        wifi_buffer_size += WIFI_AP_ADDRESS_SIZE;
    }

    /* Send buffer */
    if( wifi_mw_send_frame( wifi_result_buffer, wifi_buffer_size ) != true )
    {
        MW_DBG_TRACE_ERROR( "SEND FRAME ERROR\n" );
        return false;
    }

    return true;
}

static void wifi_mw_reset_results( void ) { memset( &wifi_results, 0, sizeof wifi_results ); }

static void wifi_mw_tx_done_callback( void )
{
    MW_DBG_TRACE_MSG( "---- internal TX DONE ----\n" );

    /* Send an event to application to notify for completion */
    wifi_mw_send_event( WIFI_MW_EVENT_TERMINATED );
}

static bool wifi_mw_send_frame( const uint8_t* tx_frame_buffer, const uint8_t tx_frame_buffer_size )
{
    smtc_modem_return_code_t modem_response_code = SMTC_MODEM_RC_OK;
    uint8_t                  tx_max_payload;
    int32_t                  duty_cycle;

    /* Sanity check:
        We expect the application parameters to be properly set to avoid:
        - exceed duty cycle
        - exceed maximum payload
        The below checks are only for developer information */

    /* Inform if duty cycle is not available */
    MW_ASSERT_SMTC_MODEM_RC( smtc_modem_get_duty_cycle_status( &duty_cycle ) );
    if( duty_cycle < 0 )
    {
        MW_DBG_TRACE_ERROR( "Duty Cycle: available for next uplink in %d milliseconds\n", duty_cycle );
    }

    /* Get the next tx payload size */
    MW_ASSERT_SMTC_MODEM_RC( smtc_modem_get_next_tx_max_payload( modem_stack_id, &tx_max_payload ) );
    if( tx_frame_buffer_size > tx_max_payload )
    {
        MW_DBG_TRACE_ERROR( "payload size: exceed max payload allowed for next uplink (%d > %d bytes)\n",
                            tx_frame_buffer_size, tx_max_payload );
    }

    /* Send uplink */
    modem_response_code =
        smtc_modem_request_extended_uplink( modem_stack_id, lorawan_port, false, tx_frame_buffer, tx_frame_buffer_size,
                                            SMTC_MODEM_EXTENDED_UPLINK_ID_WIFI, &wifi_mw_tx_done_callback );
    if( modem_response_code == SMTC_MODEM_RC_OK )
    {
        MW_DBG_TRACE_INFO( "Request uplink:\n" );
        MW_DBG_TRACE_ARRAY( "Payload", tx_frame_buffer, tx_frame_buffer_size );
        return true;
    }
    else
    {
        MW_DBG_TRACE_ERROR( "Request uplink failed with modem_response_code : %d \n", modem_response_code );
        return false;
    }
}

static void wifi_mw_send_event( wifi_mw_event_type_t event_type )
{
    /* The scan sequence ends when an event is sent to the application, except if SCAN_DONE */
    if( event_type != WIFI_MW_EVENT_SCAN_DONE )
    {
        task_running = false;
    }

    /* Send the event to the application */
    pending_events = pending_events | ( 1 << event_type );
    MW_ASSERT_SMTC_MODEM_RC( smtc_modem_increment_event_middleware( SMTC_MODEM_EVENT_MIDDLEWARE_2, pending_events ) );
}

/* --- EOF ------------------------------------------------------------------ */