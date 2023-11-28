/**
 * @file      gnss_middleware.c
 *
 * @brief     GNSS geolocation middleware implementing scan & send sequence.
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

#include <stdio.h>
#include <string.h>

#include "gnss_version.h"
#include "gnss_middleware.h"
#include "gnss_helpers.h"
#include "gnss_queue.h"

#include "mw_assert.h"
#include "mw_dbg_trace.h"

#include "lr11xx_system.h"

#include "smtc_modem_middleware_advanced_api.h"
#include "smtc_modem_hal.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/**
 * @brief Define flag to enable/disable traces during time critical sections
 */
#define GNSS_MW_FEATURE_OFF 0
#define GNSS_MW_FEATURE_ON !GNSS_MW_FEATURE_OFF

/**
 * @brief Enable/disable traces during time critical sections
 */
#ifndef GNSS_MW_DBG_TRACE_TIME_CRITICAL
#define GNSS_MW_DBG_TRACE_TIME_CRITICAL GNSS_MW_FEATURE_OFF /* Set to GNSS_MW_FEATURE_ON to enable those traces */
#endif

/**
 * @brief Print macros to be used in time critical sections
 */
#if( GNSS_MW_DBG_TRACE_TIME_CRITICAL == GNSS_MW_FEATURE_ON )
#define GNSS_MW_TIME_CRITICAL_TRACE_MSG( msg ) \
    do                                         \
    {                                          \
        MW_DBG_TRACE_PRINTF( msg );            \
    } while( 0 )

#define GNSS_MW_TIME_CRITICAL_TRACE_PRINTF( ... ) \
    do                                            \
    {                                             \
        MW_DBG_TRACE_PRINTF( __VA_ARGS__ );       \
    } while( 0 )

#else
#define GNSS_MW_TIME_CRITICAL_TRACE_MSG( msg )
#define GNSS_MW_TIME_CRITICAL_TRACE_PRINTF( ... )
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/**
 * @brief Radio planner task ID for GNSS middleware
 */
#define RP_TASK_GNSS SMTC_MODEM_RP_TASK_ID1

/**
 * @brief LoRaWAN port used for uplinks of the GNSS scan results
 */
#define GNSS_DEFAULT_UPLINK_PORT 192

/**
 * @brief Solver aiding position buffer size (1byte for TAG and 3 for position)
 */
#define SOLVER_AIDING_POSITION_SIZE 4

/**
 * @brief The LoRa Basics Modem extended uplink ID to be used for GNSS uplinks (TASK_EXTENDED_1)
 */
#define SMTC_MODEM_EXTENDED_UPLINK_ID_GNSS 1

/**
 * @brief TODO
 */
#define LR11XX_GNSS_SCALING_LATITUDE 90

/**
 * @brief TODO
 */
#define LR11XX_GNSS_SCALING_LONGITUDE 180

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/**
 * @brief The list of possible internal pending errors
 */
typedef enum
{
    GNSS_MW_ERROR_NONE,         //!< No error
    GNSS_MW_ERROR_SCAN_FAILED,  //!< Scan failed due to LR11xx error
    GNSS_MW_ERROR_NO_TIME,      //!< Scan could not be performed because no time available
    GNSS_MW_ERROR_UNKNOWN,      //!< An unknown error occurred
} gnss_mw_internal_error_t;

/**
 * @brief Description of a scan mode
 */
typedef struct
{
    uint32_t scan_group_delay;  //!< The delay between the end of a scan and the start of the next one, in seconds
    uint8_t  scan_group_size;   //!< The number of scans in the scan group
    uint8_t  scan_group_stop;   //!< The number of SVs in a scans necessary to stop the scan group
} gnss_mw_mode_desc_t;

/**
 * @brief TODO
 */
typedef enum
{
    GNSS_MW_SCAN_TYPE_AUTONOMOUS,
    GNSS_MW_SCAN_TYPE_ASSISTED,
    GNSS_MW_SCAN_TYPE_ASSISTED_FOR_AIDING_POSITION,
} gnss_mw_scan_type_t;

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

/**
 * @brief The current scan group queue to store scan results
 */
static gnss_scan_group_queue_t gnss_scan_group_queue;

/*!
 * @brief Indicates if a user update of the current assistance position is ready to be written to the LR11xx on the next
 * scan
 */
static bool user_aiding_position_update_received = false;

/*!
 * @brief The assistance position user update to be written to the LR11xx for the next scan
 */
static lr11xx_gnss_solver_assistance_position_t user_aiding_position_update;

/*!
 * @brief Indicates if a solver update of the current assistance position is ready to be written to the LR11xx on the
 * next scan
 */
static bool solver_aiding_position_update_received = false;

/*!
 * @brief The assistance position solver update to be written to the LR11xx for the next scan
 */
static uint8_t solver_aiding_position_update[SOLVER_AIDING_POSITION_SIZE];

/*!
 * @brief TODO
 */
static gnss_mw_scan_type_t current_scan_type = GNSS_MW_SCAN_TYPE_AUTONOMOUS;

/*!
 * @brief Pre-defined scan modes to be selected by the user depending on the use case (STATIC, MOBILE...)
 */
static gnss_mw_mode_desc_t modes[__GNSS_MW_MODE__SIZE] = {
    { .scan_group_delay = 15, .scan_group_size = 4, .scan_group_stop = 0 }, /* GNSS_MW_MODE_STATIC */
    { .scan_group_delay = 0, .scan_group_size = 2, .scan_group_stop = 0 },  /* GNSS_MW_MODE_MOBILE */
};

static gnss_mw_mode_desc_t aiding_position_request_mode = {
    .scan_group_delay = 0, .scan_group_size = 4, .scan_group_stop = 8
}; /* If at least 8 SVs detected on a scan, the scan group can be stopped and sent for solving */

/*!
 * @brief The index of the modes[] array to get configuration for the current scan sequence
 */
static gnss_mw_mode_t current_mode_index;

/*!
 * @brief The current pending errors (reset when a new scan sequence starts)
 */
static gnss_mw_internal_error_t pending_error = GNSS_MW_ERROR_NONE;

/*!
 * @brief The current pending events (reset by the user or when a new scan sequence starts)
 */
static uint8_t pending_events = 0;

/*!
 * @brief The selected constellations to be used
 */
static lr11xx_gnss_constellation_mask_t current_constellations = LR11XX_GNSS_GPS_MASK + LR11XX_GNSS_BEIDOU_MASK;

/*!
 * @brief The LoRaWAN port on which results uplinks are sent
 */
static uint8_t lorawan_port = GNSS_DEFAULT_UPLINK_PORT;

/*!
 * @brief Indicates if the next scan will use the same token as the previous one
 */
static bool scan_aggregate = false;

/*!
 * @brief Indicates sequence to "scan & send" or "scan only" mode
 */
static bool send_bypass = false;

/*!
 * @brief User has requested to cancel the scan that was scheduled
 */
static bool task_cancelled_by_user = false;

/*!
 * @brief The scan sequence has started
 * Set to true when the first scan of the sequence actually started (radio)
 * Set back to false when the complete sequence is terminated (all results sent)
 */
static bool task_running = false;

/*!
 * @brief LR11xx current configuration context (almanac CRC, aiding position....)
 */
static gnss_mw_scan_context_t lr11xx_scan_context;

/*!
 * @brief TODO
 */
static uint8_t aid_pos_check_buffer[5 + GNSS_RESULT_SIZE_MAX_MODE3] = { 0 }; /* | TAG | 0x00 | LAT/LON | NAV | */

/*!
 * @brief TODO
 */
static uint8_t stat_nb_scans_sent_within_current_scan_group = 0;
static bool    stat_aiding_position_check_sent              = false;
static bool    stat_indoor_detected                         = false;

/*!
 * @brief TODO
 */
static bool autonomous_scan_for_indoor_check = false;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*!
 * @brief Program the next scan of the scan group, with the specified delay
 *
 * @param [in] delay_s Delay in seconds to start the scan
 *
 * @return the error code as returned by the modem / radio planner
 */
static smtc_modem_return_code_t gnss_mw_scan_next( uint32_t delay_s );

/*!
 * @brief TODO
 */
static uint32_t gnss_mw_get_next_scan_delay( void );

/*!
 * @brief Interrupt handler signaled by the Radio Planner when the radio is available and it is time to start the scan
 * (WARNING: running under interrupt context)
 *
 * @param [in] context Pointer to context given by RP (not used)
 */
static void gnss_mw_scan_rp_task_launch( void* context );

/*!
 * @brief Interrupt handler signaled by the Radio Planner when the scan is completed (WARNING: running under interrupt
 * context)
 *
 * @param [in] status IRQ status from RP
 */
static void gnss_mw_scan_rp_task_done( smtc_modem_rp_status_t* status );

/*!
 * @brief Send an uplink with the current aiding position to LoRaCloud, for checking if possible and send a downlink
 * update if necessary.
 *
 * @param [in] current_assistance_position The current aiding position configured
 *
 * @return a boolean set to true for success, false otherwise
 */
static bool gnss_mw_send_aiding_position_check_request(
    const lr11xx_gnss_solver_assistance_position_t* current_assistance_position, gnss_scan_t* scan );

/*!
 * @brief Callback called by the LBM when the uplink has been sent. Pop the next result ot be sent until the scan group
 * queue is empty.
 */
static void gnss_mw_tx_done_callback( void );

/*!
 * @brief Request an uplink to LBM through the extended API (no buffer copy)
 *
 * @param [in] tx_frame_buffer A pointer to the buffer payload to be sent over the air.
 * @param [in] tx_frame_buffer_size The size of the buffer to be sent.
 *
 * @return a boolean set to true for success, false otherwise.
 */
static bool gnss_mw_send_frame( const uint8_t* tx_frame_buffer, const uint8_t tx_frame_buffer_size, uint8_t port );

/*!
 * @brief Add an event to the pending event bitfield, and send all pending events to the application
 *
 * @param [in] event_type The event to be added to the pending events bitfield.
 */
static void gnss_mw_send_event( gnss_mw_event_type_t event_type );

/**
 * @brief Forward the aiding position received from the solver (through a downlink) to the LR11xx
 *
 * @param [in] payload Aiding position payload as transmitted by the solver
 * @param [in] size Size of the aiding position payload
 */
static void gnss_mw_set_solver_aiding_position( const uint8_t* payload, uint8_t size );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

mw_return_code_t gnss_mw_get_version( mw_version_t* version )
{
    if( version == NULL )
    {
        MW_DBG_TRACE_ERROR( "Failed to get GNSS middleware version\n" );
        return MW_RC_FAILED;
    }

    version->major = GNSS_MW_VERSION_MAJOR;
    version->minor = GNSS_MW_VERSION_MINOR;
    version->patch = GNSS_MW_VERSION_PATCH;

    return MW_RC_OK;
}

mw_return_code_t gnss_mw_init( ralf_t* modem_radio, uint8_t stack_id )
{
    if( modem_radio == NULL )
    {
        MW_DBG_TRACE_ERROR( "Failed to init GNSS middleware: modem radio not set\n" );
        return MW_RC_FAILED;
    }

    /* Initialize GNSS scan group queue */
    memset( &gnss_scan_group_queue, 0, sizeof( gnss_scan_group_queue_t ) );
    gnss_scan_group_queue_reset_token( &gnss_scan_group_queue );

    /* Set radio context */
    modem_radio_ctx = modem_radio;

    /* Set modem stack ID */
    modem_stack_id = stack_id;

    return MW_RC_OK;
}

mw_return_code_t gnss_mw_scan_start( gnss_mw_mode_t mode, uint32_t start_delay )
{
    smtc_modem_return_code_t modem_rc;
    bool                     scan_group_err;

    if( modem_radio_ctx == NULL )
    {
        MW_DBG_TRACE_ERROR( "GNSS middleware not ready, cannot start scan\n" );
        return MW_RC_FAILED;
    }

    if( mode >= __GNSS_MW_MODE__SIZE )
    {
        MW_DBG_TRACE_ERROR( "Wrong parameter, mode %d is not supported\n", mode );
        return MW_RC_FAILED;
    }

    /* Check a sequence sequence is already running (meaning the RP task has been launched) */
    if( task_running == true )
    {
        MW_DBG_TRACE_ERROR( "GNSS scan on-going, cannot start a new scan yet\n" );
        return MW_RC_BUSY;
    }

    /* Set selected mode */
    current_mode_index = mode;

    /* Reset pending internal error */
    pending_error = GNSS_MW_ERROR_NONE;

    /* Reset pending events */
    pending_events = 0;

    /* Reset any pending cancel request which has not been completed (error case) */
    task_cancelled_by_user = false;

    /* Reset scan context */
    memset( &lr11xx_scan_context, 0, sizeof lr11xx_scan_context );

    /* Reset stats */
    stat_nb_scans_sent_within_current_scan_group = 0;
    stat_aiding_position_check_sent              = false;
    stat_indoor_detected                         = false;

    /* Switch back to assisted if previous scan was for aiding position request */
    if( current_scan_type == GNSS_MW_SCAN_TYPE_ASSISTED_FOR_AIDING_POSITION )
    {
        current_scan_type = GNSS_MW_SCAN_TYPE_ASSISTED;
    }

    /* Initialize new scan group */
    MW_DBG_TRACE_INFO( "New scan group - type:%d - start_delay:%us\n", current_scan_type, start_delay );
    if( current_scan_type != GNSS_MW_SCAN_TYPE_ASSISTED )
    {
        scan_group_err =
            gnss_scan_group_queue_new( &gnss_scan_group_queue, aiding_position_request_mode.scan_group_size,
                                       aiding_position_request_mode.scan_group_stop );
    }
    else
    {
        scan_group_err = gnss_scan_group_queue_new( &gnss_scan_group_queue, modes[current_mode_index].scan_group_size,
                                                    modes[current_mode_index].scan_group_stop );
    }
    if( scan_group_err != true )
    {
        MW_DBG_TRACE_ERROR( "Failed to create scan group queue\n" );
        return MW_RC_FAILED;
    }

    /* Prepare the task for next scan */
    modem_rc = gnss_mw_scan_next( start_delay );
    if( modem_rc != SMTC_MODEM_RC_OK )
    {
        return MW_RC_FAILED;
    }

    return MW_RC_OK;
}

mw_return_code_t gnss_mw_scan_cancel( void )
{
    smtc_modem_return_code_t modem_rc;

    if( modem_radio_ctx == NULL )
    {
        MW_DBG_TRACE_ERROR( "GNSS middleware not ready, no scan to cancel\n" );
        return MW_RC_FAILED;
    }

    /* The GNSS scan sequence will be in running state from the moment the task
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
        MW_DBG_TRACE_ERROR( "GNSS scan sequence started, too late to cancel\n" );
        return MW_RC_BUSY;
    }

    task_cancelled_by_user = true;

    MW_DBG_TRACE_INFO( "RP_TASK_GNSS - Request cancel of scheduled scan\n" );
    modem_rc = smtc_modem_rp_abort_user_radio_access_task( RP_TASK_GNSS );
    if( modem_rc != SMTC_MODEM_RC_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to abort GNSS scan task\n" );
    }

    return MW_RC_OK;
}

mw_return_code_t gnss_mw_set_user_aiding_position( float latitude, float longitude )
{
    /* Store the user assistance position to be written to the LR11xx on the next scan */
    user_aiding_position_update.latitude  = latitude;
    user_aiding_position_update.longitude = longitude;
    user_aiding_position_update_received  = true;

    /* We can switch to assisted scan for the next scan */
    current_scan_type = GNSS_MW_SCAN_TYPE_ASSISTED;

    return MW_RC_OK;
}

bool gnss_mw_has_event( uint8_t pending_events, gnss_mw_event_type_t event )
{
    if( ( pending_events & ( 1 << event ) ) == ( 1 << event ) )
    {
        return true;
    }

    return false;
}

mw_return_code_t gnss_mw_get_event_data_scan_done( gnss_mw_event_data_scan_done_t* data )
{
    if( data == NULL )
    {
        MW_DBG_TRACE_ERROR( "Provided pointer is NULL\n" );
        return MW_RC_FAILED;
    }

    if( gnss_mw_has_event( pending_events, GNSS_MW_EVENT_SCAN_DONE ) )
    {
        data->is_valid       = gnss_scan_group_queue_is_valid( &gnss_scan_group_queue );
        data->token          = gnss_scan_group_queue.token;
        data->nb_scans_valid = gnss_scan_group_queue.nb_scans_valid;
        /* Note: nb_scans_valid is <= GNSS_SCAN_GROUP_SIZE_MAX */
        for( uint8_t i = 0; i < gnss_scan_group_queue.nb_scans_valid; i++ )
        {
            data->scans[i].nav       = &gnss_scan_group_queue.scans[i].results_buffer[GNSS_SCAN_METADATA_SIZE];
            data->scans[i].nav_size  = gnss_scan_group_queue.scans[i].results_size;
            data->scans[i].nav_valid = gnss_scan_group_queue.scans[i].nav_valid;
            data->scans[i].timestamp = gnss_scan_group_queue.scans[i].timestamp;
            data->scans[i].nb_svs    = gnss_scan_group_queue.scans[i].detected_svs;
            /* Note: detected_sv is <= GNSS_NB_SVS_MAX */
            for( uint8_t j = 0; j < gnss_scan_group_queue.scans[i].detected_svs; j++ )
            {
                data->scans[i].info_svs[j].sv_id = gnss_scan_group_queue.scans[i].info_svs[j].satellite_id;
                data->scans[i].info_svs[j].cnr   = gnss_scan_group_queue.scans[i].info_svs[j].cnr;
            }
        }
        data->power_consumption_uah             = gnss_scan_group_queue.power_consumption_uah;
        data->context.mode                      = lr11xx_scan_context.mode;
        data->context.assisted                  = lr11xx_scan_context.assisted;
        data->context.aiding_position_latitude  = lr11xx_scan_context.aiding_position_latitude;
        data->context.aiding_position_longitude = lr11xx_scan_context.aiding_position_longitude;
        data->context.almanac_crc               = lr11xx_scan_context.almanac_crc;
        data->context.almanac_update_required   = lr11xx_scan_context.almanac_update_required;

        return MW_RC_OK;
    }
    else
    {
        MW_DBG_TRACE_ERROR( "Data are not ready\n" );
        return MW_RC_FAILED;
    }
}

void gnss_mw_set_constellations( gnss_mw_constellation_t constellations )
{
    if( constellations == GNSS_MW_CONSTELLATION_GPS )
    {
        current_constellations = LR11XX_GNSS_GPS_MASK;
    }
    else if( constellations == GNSS_MW_CONSTELLATION_BEIDOU )
    {
        current_constellations = LR11XX_GNSS_BEIDOU_MASK;
    }
    else
    {
        current_constellations = LR11XX_GNSS_GPS_MASK + LR11XX_GNSS_BEIDOU_MASK;
    }
}

void gnss_mw_set_port( uint8_t port ) { lorawan_port = port; }

void gnss_mw_scan_aggregate( bool aggregate )
{
    MW_DBG_TRACE_INFO( "GNSS scan: set aggregate mode to %s\n", aggregate ? "TRUE" : "FALSE" );

    /* Set scan aggregation current mode */
    scan_aggregate = aggregate;
}

void gnss_mw_send_bypass( bool no_send )
{
    MW_DBG_TRACE_INFO( "GNSS scan: set scan only mode to %s (bypass send)\n", no_send ? "TRUE" : "FALSE" );

    /* Set scan only current mode */
    send_bypass = no_send;
}

void gnss_mw_display_results( const gnss_mw_event_data_scan_done_t* data )
{
    uint8_t i, j;

    if( data != NULL )
    {
        MW_DBG_TRACE_PRINTF( "SCAN_DONE info:\n" );
        MW_DBG_TRACE_PRINTF( "-- token: 0x%02X\n", data->token );
        MW_DBG_TRACE_PRINTF( "-- is_valid: %d\n", data->is_valid );
        MW_DBG_TRACE_PRINTF( "-- number of valid scans: %u\n", data->nb_scans_valid );
        for( i = 0; i < data->nb_scans_valid; i++ )
        {
            MW_DBG_TRACE_PRINTF( "-- scan[%d][%u] (%u SV - %d): ", i, data->scans[i].timestamp, data->scans[i].nb_svs,
                                 data->scans[i].nav_valid );
            for( j = 0; j < data->scans[i].nav_size; j++ )
            {
                MW_DBG_TRACE_PRINTF( "%02X", data->scans[i].nav[j] );
            }
            MW_DBG_TRACE_PRINTF( "\n" );
            for( j = 0; j < data->scans[i].nb_svs; j++ )
            {
                MW_DBG_TRACE_PRINTF( "   SV_ID %u:\t%ddB\n", data->scans[i].info_svs[j].sv_id,
                                     data->scans[i].info_svs[j].cnr );
            }
        }
        MW_DBG_TRACE_PRINTF( "-- power consumption: %u uah\n", data->power_consumption_uah );
        MW_DBG_TRACE_PRINTF( "-- mode: %d\n", data->context.mode );
        MW_DBG_TRACE_PRINTF( "-- assisted: %d\n", data->context.assisted );
        if( data->context.assisted == true )
        {
            MW_DBG_TRACE_PRINTF( "-- aiding position: (%.6f, %.6f)\n", data->context.aiding_position_latitude,
                                 data->context.aiding_position_longitude );
        }
        MW_DBG_TRACE_PRINTF( "-- almanac CRC: 0X%08X\n", data->context.almanac_crc );
        MW_DBG_TRACE_PRINTF( "-- almanac update required: %d\n", data->context.almanac_update_required );
    }
}

mw_return_code_t gnss_mw_get_event_data_terminated( gnss_mw_event_data_terminated_t* data )
{
    if( data == NULL )
    {
        MW_DBG_TRACE_ERROR( "Provided pointer is NULL\n" );
        return MW_RC_FAILED;
    }

    if( gnss_mw_has_event( pending_events, GNSS_MW_EVENT_TERMINATED ) )
    {
        data->nb_scans_sent              = stat_nb_scans_sent_within_current_scan_group;
        data->aiding_position_check_sent = stat_aiding_position_check_sent;
        data->indoor_detected            = stat_indoor_detected;
        return MW_RC_OK;
    }
    else
    {
        MW_DBG_TRACE_ERROR( "Scan is not terminated\n" );
        return MW_RC_FAILED;
    }
}

void gnss_mw_clear_pending_events( void ) { pending_events = 0; }

mw_return_code_t gnss_mw_handle_downlink( uint8_t port, const uint8_t* payload, uint8_t size )
{
    /* Check if this downlink is for the GNSS middleware, otherwise, just ignore it */
    if( port != lorawan_port )
    {
        return MW_RC_OK;
    }

    /* Check input parameters */
    if( payload == NULL )
    {
        MW_DBG_TRACE_ERROR( "Downlink payload pointer is NULL\n" );
        return MW_RC_FAILED;
    }
    if( size < SOLVER_AIDING_POSITION_SIZE )
    {
        MW_DBG_TRACE_ERROR( "Downlink payload size not supported\n" );
        return MW_RC_FAILED;
    }

    /* Check if the payload contains an D-GNSSLOC-AIDP: Aiding Position as defined by LoRaCloud */
    if( payload[0] == 0x00 )
    {
        MW_DBG_TRACE_INFO( "Received D-GNSSLOC-AIDP solver message\n" );
        gnss_mw_set_solver_aiding_position( payload, SOLVER_AIDING_POSITION_SIZE );
    }
    else
    {
        MW_DBG_TRACE_ERROR( "Unknown downlink type for GNSS: 0x%02X\n", payload[0] );
        return MW_RC_FAILED;
    }

    /* Note: The remaining bytes (if any) will be ignored. In the future, we could support several messages in the
     * payload */

    return MW_RC_OK;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

static smtc_modem_return_code_t gnss_mw_scan_next( uint32_t delay_s )
{
    smtc_modem_rp_task_t     rp_task = { 0 };
    smtc_modem_return_code_t modem_rc;
    uint32_t                 time_ms, delay_ms;

    time_ms  = smtc_modem_hal_get_time_in_ms( ) + 300; /* 300ms for scheduling delay */
    delay_ms = delay_s * 1000;

    rp_task.type                 = SMTC_MODEM_RP_TASK_STATE_ASAP;
    rp_task.start_time_ms        = time_ms + delay_ms;
    rp_task.duration_time_ms     = 10 * 1000;
    rp_task.id                   = RP_TASK_GNSS;
    rp_task.launch_task_callback = gnss_mw_scan_rp_task_launch;
    rp_task.end_task_callback    = gnss_mw_scan_rp_task_done;
    modem_rc                     = smtc_modem_rp_add_user_radio_access_task( &rp_task );
    if( modem_rc == SMTC_MODEM_RC_OK )
    {
        GNSS_MW_TIME_CRITICAL_TRACE_PRINTF( "RP_TASK_GNSS - scan task queued at %u + %u\n", time_ms, delay_ms );
    }
    else
    {
        MW_DBG_TRACE_ERROR( "RP_TASK_GNSS - failed to queue scan task (0x%02X)\n", modem_rc );
    }

    return modem_rc;
}

static uint32_t gnss_mw_get_next_scan_delay( void )
{
    if( current_scan_type == GNSS_MW_SCAN_TYPE_ASSISTED )
    {
        return modes[current_mode_index].scan_group_delay;
    }
    else
    {
        return aiding_position_request_mode.scan_group_delay;
    }
}

static void gnss_mw_scan_rp_task_launch( void* context )
{
    smtc_modem_return_code_t                 err;
    uint32_t                                 gps_time           = 0;
    uint32_t                                 fractional_seconds = 0;
    lr11xx_gnss_solver_assistance_position_t lr11xx_aiding_position;
    smtc_gnss_scan_params_t                  scan_params;
    gnss_mw_scan_type_t                      scan_type =
        ( ( autonomous_scan_for_indoor_check == true ) ? GNSS_MW_SCAN_TYPE_AUTONOMOUS : current_scan_type );

    /* From now, the scan sequence can not be cancelled */
    task_running = true;

    MW_DBG_TRACE_PRINTF( "---- internal scan start (%d) ----\n", scan_type );

    err = smtc_modem_get_time( &gps_time, &fractional_seconds );
    if( err == SMTC_MODEM_RC_OK )
    {
        /* Set assistance position if an update is pending */
        if( user_aiding_position_update_received == true )
        {
            if( smtc_gnss_set_assistance_position( modem_radio_ctx->ral.context, &user_aiding_position_update ) ==
                true )
            {
                GNSS_MW_TIME_CRITICAL_TRACE_PRINTF( "User assistance position set to (%.6f, %.6f)\n",
                                                    user_aiding_position_update.latitude,
                                                    user_aiding_position_update.longitude );
                user_aiding_position_update_received = false;
            }
        }
        if( solver_aiding_position_update_received == true )
        {
            if( smtc_gnss_push_solver_msg( modem_radio_ctx->ral.context, solver_aiding_position_update,
                                           SOLVER_AIDING_POSITION_SIZE ) == true )
            {
                GNSS_MW_TIME_CRITICAL_TRACE_PRINTF( "Solver assistance position set to " );
                for( int i = 0; i < SOLVER_AIDING_POSITION_SIZE; i++ )
                {
                    GNSS_MW_TIME_CRITICAL_TRACE_PRINTF( "%02X ", solver_aiding_position_update[i] );
                }
                GNSS_MW_TIME_CRITICAL_TRACE_PRINTF( "\n" );
                solver_aiding_position_update_received = false;
            }
        }

        /* Get context used for scan */
        smtc_gnss_get_scan_context( modem_radio_ctx->ral.context, &lr11xx_aiding_position,
                                    &lr11xx_scan_context.almanac_crc );
        lr11xx_scan_context.aiding_position_latitude  = lr11xx_aiding_position.latitude;
        lr11xx_scan_context.aiding_position_longitude = lr11xx_aiding_position.longitude;
        lr11xx_scan_context.assisted                  = ( scan_type == GNSS_MW_SCAN_TYPE_AUTONOMOUS ) ? false : true;
        lr11xx_scan_context.mode                      = current_mode_index;
        lr11xx_scan_context.gps_time                  = gps_time;

        /* Set GNSS scan parameters */
        scan_params.constellations = current_constellations;
        switch( scan_type )
        {
        case GNSS_MW_SCAN_TYPE_AUTONOMOUS:
            scan_params.assisted          = false;
            scan_params.freq_search_space = LR11XX_GNSS_FREQUENCY_SEARCH_SPACE_250_HZ; /* not used */
            scan_params.input_parameters  = LR11XX_GNSS_RESULTS_DOPPLER_MASK +
                                           LR11XX_GNSS_RESULTS_DOPPLER_ENABLE_MASK; /* 14 dopplers max, no bit change */
            scan_params.nb_svs_max =
                9; /* NAV2: 9 SV + 9 dopplers => 44 bytes (reduce nb SV to keep space for APC message)*/
            break;
        case GNSS_MW_SCAN_TYPE_ASSISTED:
            scan_params.assisted          = true;
            scan_params.freq_search_space = LR11XX_GNSS_FREQUENCY_SEARCH_SPACE_250_HZ;
            scan_params.input_parameters  = 0; /* no dopplers, no bit change */
            scan_params.nb_svs_max        = GNSS_NB_SVS_MAX;
            break;
        case GNSS_MW_SCAN_TYPE_ASSISTED_FOR_AIDING_POSITION:
            scan_params.assisted          = true;
            scan_params.freq_search_space = LR11XX_GNSS_FREQUENCY_SEARCH_SPACE_2_KHZ;
            scan_params.input_parameters  = LR11XX_GNSS_RESULTS_DOPPLER_MASK +
                                           LR11XX_GNSS_RESULTS_DOPPLER_ENABLE_MASK; /* 14 dopplers max, no bit change */
            scan_params.nb_svs_max =
                9; /* NAV2: 9 SV + 9 dopplers => 47 bytes (reduce nb SV to keep payload under 50 bytes) */
            break;
        default:
            break;
        }

        /* Start GNSS scan */
        if( smtc_gnss_scan( modem_radio_ctx->ral.context, gps_time, &scan_params ) != true )
        {
            pending_error = GNSS_MW_ERROR_SCAN_FAILED;

            MW_DBG_TRACE_ERROR( "RP_TASK_GNSS - failed to start scan, abort task\n" );
            MW_ASSERT_SMTC_MODEM_RC( smtc_modem_rp_abort_user_radio_access_task( RP_TASK_GNSS ) );

            /*
            When aborting the task, the RP will call the end_task_callback() with SMTC_RP_RADIO_ABORTED status.
            ERROR event will be sent from there to the application
            */
        }
    }
    else if( err == SMTC_MODEM_RC_NO_TIME )
    {
        pending_error = GNSS_MW_ERROR_NO_TIME;

        MW_DBG_TRACE_ERROR( "RP_TASK_GNSS - time sync is not valid, abort task\n" );
        MW_ASSERT_SMTC_MODEM_RC( smtc_modem_rp_abort_user_radio_access_task( RP_TASK_GNSS ) );

        /*
        When aborting the task, the RP will call the end_task_callback() with SMTC_RP_RADIO_ABORTED status.
        ERROR event will be sent from there to the application
        */
    }
    else
    {
        pending_error = GNSS_MW_ERROR_UNKNOWN;

        MW_DBG_TRACE_ERROR( "RP_TASK_GNSS - failed to get time, abort task\n" );
        MW_ASSERT_SMTC_MODEM_RC( smtc_modem_rp_abort_user_radio_access_task( RP_TASK_GNSS ) );

        /*
        When aborting the task, the RP will call the end_task_callback() with SMTC_RP_RADIO_ABORTED status.
        ERROR event will be sent from there to the application
        */
    }
}

void gnss_mw_scan_rp_task_done( smtc_modem_rp_status_t* status )
{
    smtc_modem_rp_radio_status_t irq_status = status->status;
    uint32_t                     time_ms;
    uint32_t                     meas_time;
    uint32_t                     power_consumption_uah;

    /* -------------------------------------------------------------------------
       WARNING: put the radio back to sleep before exiting this function.
       ---------------------------------------------------------------------- */

    /* -------------------------------------------------------------------------
       WARNING: this callback function is called by the radio planner under
                interrupt context. Exit as fast as possible.
       ---------------------------------------------------------------------- */

    time_ms = smtc_modem_hal_get_time_in_ms( );
    GNSS_MW_TIME_CRITICAL_TRACE_PRINTF( "GNSS task done at %d (irq_status=%d)\n", time_ms, irq_status );

    /* GNSS task completed or aborted - first thing to be done */
    smtc_gnss_scan_ended( );

    if( irq_status == SMTC_RP_RADIO_ABORTED ) /* by RP or by user */
    {
        if( pending_error == GNSS_MW_ERROR_NONE )
        {
            if( task_cancelled_by_user == false )
            {
                MW_DBG_TRACE_WARNING( "RP_TASK_GNSS(%d) - task aborted by RP\n", __LINE__ );
                /* Program next GNSS scan */
                MW_ASSERT_SMTC_MODEM_RC( gnss_mw_scan_next( gnss_mw_get_next_scan_delay( ) ) );
            }
            else
            {
                MW_DBG_TRACE_WARNING( "RP_TASK_GNSS(%d) - task cancelled by user\n", __LINE__ );

                /* We handle here the fact that when the user requests to abort a radio planner task
                with smtc_modem_rp_abort_user_radio_access_task(), the RP will call this task_done callback
                with status set to SMTC_RP_RADIO_ABORTED */

                /* reset cancel request status */
                task_cancelled_by_user = false;

                /* Send an event to application to notify for error */
                gnss_mw_send_event( GNSS_MW_EVENT_SCAN_CANCELLED );
            }
        }
        else if( pending_error == GNSS_MW_ERROR_NO_TIME )
        {
            MW_DBG_TRACE_WARNING( "RP_TASK_GNSS(%d) - task aborted NO_TIME\n", __LINE__ );

            /* Send an event to application to notify for error */
            gnss_mw_send_event( GNSS_MW_EVENT_ERROR_NO_TIME );
        }
        else
        {
            MW_DBG_TRACE_WARNING( "RP_TASK_GNSS(%d) - task aborted for UNKNOWN reason\n", __LINE__ );

            /* Send an event to application to notify for error */
            gnss_mw_send_event( GNSS_MW_EVENT_ERROR_UNKNOWN );
        }
    }
    else if( irq_status == SMTC_RP_RADIO_GNSS_SCAN_DONE )
    {
        gnss_scan_t                              scan_results = { 0 };
        smtc_gnss_get_results_return_code_t      scan_results_rc;
        bool                                     scan_results_no_sv      = false;
        bool                                     doppler_error           = false;
        bool                                     almanac_update_required = false;
        lr11xx_gnss_solver_assistance_position_t current_assistance_position;

        current_assistance_position.latitude  = lr11xx_scan_context.aiding_position_latitude;
        current_assistance_position.longitude = lr11xx_scan_context.aiding_position_longitude;

        /* Get scan results from LR1110 */
        scan_results.timestamp = mw_get_gps_time( );
        scan_results_rc =
            smtc_gnss_get_results( modem_radio_ctx->ral.context, GNSS_RESULT_SIZE_MAX_MODE3, &scan_results.results_size,
                                   &scan_results.results_buffer[GNSS_SCAN_METADATA_SIZE], &scan_results_no_sv );

        /* Get scan power consumption and aggregate it to the scan group power consumption */
        smtc_gnss_get_power_consumption( modem_radio_ctx->ral.context, &power_consumption_uah );
        GNSS_MW_TIME_CRITICAL_TRACE_PRINTF( "Scan power consumption: %u uah\n", power_consumption_uah );
        gnss_scan_group_queue.power_consumption_uah += power_consumption_uah;

        if( scan_results_rc == SMTC_GNSS_GET_RESULTS_NO_ERROR )
        {
            /* Get detailed info about the scan */
            smtc_gnss_get_sv_info( modem_radio_ctx->ral.context, GNSS_NB_SVS_MAX, &scan_results.detected_svs,
                                   scan_results.info_svs );

            if( autonomous_scan_for_indoor_check == true )
            {
                /* The scan which just completed was for an indoor / aiding position check */
                autonomous_scan_for_indoor_check = false;

                if( scan_results_no_sv == true )
                {
                    /* If NO SATELLITES has been detected, we consider the device is indoor */
                    stat_indoor_detected = true;
                    gnss_mw_send_event( GNSS_MW_EVENT_TERMINATED );
                }
                else
                {
                    /* The autonomous scan detected some SVs but not enough to generate a NAV, so send an aiding
                     * position check uplink */
                    stat_aiding_position_check_sent = true;
                    gnss_mw_send_aiding_position_check_request( &current_assistance_position, &scan_results );
                }
            }
            else
            {
                /* Get almanac update status (once per scan group) */
                if( lr11xx_scan_context.almanac_update_checked == false )
                {
                    smtc_gnss_get_almanac_update_status( modem_radio_ctx->ral.context, lr11xx_scan_context.gps_time,
                                                         &current_assistance_position, current_constellations,
                                                         &almanac_update_required );
                    /* Update scan context */
                    lr11xx_scan_context.almanac_update_checked  = true;
                    lr11xx_scan_context.almanac_update_required = almanac_update_required;
                }

                /* Check if there is doppler error for detected satellites (if assisted scan) */
                if( ( scan_results.detected_svs > 0 ) && ( current_scan_type != GNSS_MW_SCAN_TYPE_AUTONOMOUS ) &&
                    ( lr11xx_scan_context.almanac_update_required == false ) )
                {
                    smtc_gnss_get_doppler_error(
                        modem_radio_ctx->ral.context, lr11xx_scan_context.gps_time, &current_assistance_position,
                        current_constellations, scan_results.detected_svs, scan_results.info_svs,
                        &doppler_error ); /* TODO: need to add constellations to scan context */
                }

                if( ( doppler_error == true ) &&
                    ( current_scan_type != GNSS_MW_SCAN_TYPE_ASSISTED_FOR_AIDING_POSITION ) )
                {
                    current_scan_type = GNSS_MW_SCAN_TYPE_ASSISTED_FOR_AIDING_POSITION;

                    /* Save power consumption */
                    uint32_t power_consumption_uah_save = gnss_scan_group_queue.power_consumption_uah;

                    /* Clear current queue and reconfigure */
                    gnss_scan_group_queue_new( &gnss_scan_group_queue, aiding_position_request_mode.scan_group_size,
                                               aiding_position_request_mode.scan_group_stop );

                    /* Restore power consumption */
                    gnss_scan_group_queue.power_consumption_uah = power_consumption_uah_save;

                    /* Program next GNSS scan */
                    MW_ASSERT_SMTC_MODEM_RC( gnss_mw_scan_next( 0 ) );
                }
                else
                {
                    /* Check if the NAV message is valid (the solver can use this single NAV to get a position) */
                    scan_results.nav_valid = smtc_gnss_is_nav_message_valid(
                        current_constellations, scan_results.detected_svs, scan_results.info_svs );

                    /* Push scan to the scan group */
                    gnss_scan_group_queue_push( &gnss_scan_group_queue, &scan_results );

                    /* Trigger next GNSS scan or send first scan results, if scan group completed */
                    if( gnss_scan_group_queue_is_full( &gnss_scan_group_queue ) == false )
                    {
                        /* Program next GNSS scan */
                        MW_ASSERT_SMTC_MODEM_RC( gnss_mw_scan_next( gnss_mw_get_next_scan_delay( ) ) );
                    }
                    else
                    {
                        /* All scans in the group have been completed, send an event to application */
                        gnss_mw_send_event( GNSS_MW_EVENT_SCAN_DONE );

                        /* Send results */
                        /* static variables because there is no copy done by LBM for extended send API */
                        static uint8_t* buffer_to_send;
                        static uint8_t  buffer_to_send_size;

                        /* Is there any scan to be sent ? */
                        if( gnss_scan_group_queue_pop( &gnss_scan_group_queue, &buffer_to_send,
                                                       &buffer_to_send_size ) == false )
                        {
                            /* No SV detected for this scan group, program an autonomous scan for indoor check / aiding
                             * position check */
                            autonomous_scan_for_indoor_check = true;
                            MW_ASSERT_SMTC_MODEM_RC( gnss_mw_scan_next( 0 ) );
                            /* TODO: if current scan was an autonomous one, we could avoid this last one for indoor
                             * check */
                        }
                        else
                        {
                            /* Check if "no send "mode" is configured */
                            if( send_bypass == true )
                            {
                                /* Send an event to application to notify for completion */
                                /* The application needs to know that it can proceed with the next scan */
                                gnss_mw_send_event( GNSS_MW_EVENT_TERMINATED );
                            }
                            else
                            {
                                /* Send uplink */
                                if( gnss_mw_send_frame( buffer_to_send, buffer_to_send_size, lorawan_port ) == false )
                                {
                                    MW_DBG_TRACE_ERROR( "Failed to send uplink frame\n" );
                                    /* TODO: send error event ? */
                                }
                                else
                                {
                                    stat_nb_scans_sent_within_current_scan_group += 1;
                                }
                            }
                        }
                    }
                }
            }
        }
        else if( scan_results_rc == SMTC_GNSS_GET_RESULTS_ERROR_ALMANAC )
        {
            MW_DBG_TRACE_ERROR( "RP_TASK_GNSS - almanac update required\n" );
            /* Send an event to application to notify for error */
            gnss_mw_send_event( GNSS_MW_EVENT_ERROR_ALMANAC_UPDATE );
        }
        else if( scan_results_rc == SMTC_GNSS_GET_RESULTS_ERROR_AIDING_POS )
        {
            MW_DBG_TRACE_ERROR( "RP_TASK_GNSS - no assistance position configured\n" );
            /* Send an event to application to notify for error */
            gnss_mw_send_event( GNSS_MW_EVENT_ERROR_NO_AIDING_POSITION );
        }
        else if( scan_results_rc == SMTC_GNSS_GET_RESULTS_ERROR_NO_TIME )
        {
            MW_DBG_TRACE_ERROR( "RP_TASK_GNSS - no valid time available\n" );
            /* Send an event to application to notify for error */
            gnss_mw_send_event( GNSS_MW_EVENT_ERROR_NO_TIME );
        }
        else
        {
            MW_DBG_TRACE_ERROR( "RP_TASK_GNSS - unkown error on get results\n" );
            /* Send an event to application to notify for error */
            gnss_mw_send_event( GNSS_MW_EVENT_ERROR_UNKNOWN );
        }
    }
    else
    {
        MW_DBG_TRACE_ERROR( "GNSS RP task - Unknown status %d at %d\n", irq_status, time_ms );

        /* Send an event to application to notify for error */
        gnss_mw_send_event( GNSS_MW_EVENT_ERROR_UNKNOWN );
    }

    /* Monitor callback exec duration (should be kept as low as possible) */
    meas_time = smtc_modem_hal_get_time_in_ms( );
    MW_DBG_TRACE_WARNING( "GNSS RP task - done callback duration %u ms\n", meas_time - time_ms );

    /* Set the radio back to sleep */
    mw_radio_set_sleep( modem_radio_ctx->ral.context );
}

static bool gnss_mw_send_aiding_position_check_request(
    const lr11xx_gnss_solver_assistance_position_t* current_assistance_position, gnss_scan_t* scan )
{
    bool          success   = false;
    const int16_t latitude  = ( ( current_assistance_position->latitude * 2048 ) / LR11XX_GNSS_SCALING_LATITUDE );
    const int16_t longitude = ( ( current_assistance_position->longitude * 2048 ) / LR11XX_GNSS_SCALING_LONGITUDE );

    /* Prepare buffer for request */
    /* | TAG (8b) | 0x00 | LatLSB (8b) | LonLSB (4b) LatMSB (4b) | LonMSB (8b) | */
    aid_pos_check_buffer[0] = ( scan->detected_svs > 0 ) ? 0x01 : 0x00; /* TAG */
    aid_pos_check_buffer[1] = 0x00;                                     /* Extension marker */

    /* Current aiding position */
    aid_pos_check_buffer[2] = latitude & 0xFF;
    aid_pos_check_buffer[3] = ( ( longitude & 0x00F ) << 4 ) | ( ( latitude & 0xF00 ) >> 8 );
    aid_pos_check_buffer[4] = ( longitude & 0xFF0 ) >> 4;

    /* Append NAV if available */
    if( scan->detected_svs > 0 )
    {
        /* The number of SV max has been limited for autonomous scan in order to keep the complete buffer below 51 bytes
         */
        memcpy( &aid_pos_check_buffer[5], &( scan->results_buffer[GNSS_SCAN_METADATA_SIZE] ), scan->results_size );
    }

    /* Send uplink */
    if( gnss_mw_send_frame( aid_pos_check_buffer, 5 + scan->results_size, lorawan_port ) == true )
    {
        success = true;
    }
    else
    {
        MW_DBG_TRACE_ERROR( "Failed to send aiding position request uplink frame\n" );
    }

    return success;
}

static void gnss_mw_tx_done_callback( void )
{
    MW_DBG_TRACE_MSG( "---- internal TX DONE ----\n" );

    /* static variables because there is no copy done by LBM for extended send API */
    static uint8_t* buffer_to_send;
    static uint8_t  buffer_to_send_size;

    /* Get the next scan to be sent from the scan group queue */
    if( gnss_scan_group_queue_pop( &gnss_scan_group_queue, &buffer_to_send, &buffer_to_send_size ) == true )
    {
        /* Send uplink */
        if( gnss_mw_send_frame( buffer_to_send, buffer_to_send_size, lorawan_port ) == false )
        {
            MW_DBG_TRACE_ERROR( "Failed to send uplink frame\n" );
            /* TODO: send error event ? */
        }
        else
        {
            stat_nb_scans_sent_within_current_scan_group += 1;
        }
    }
    else
    {
        /* Send an event to application to notify for completion */
        gnss_mw_send_event( GNSS_MW_EVENT_TERMINATED );
    }
}

static bool gnss_mw_send_frame( const uint8_t* tx_frame_buffer, const uint8_t tx_frame_buffer_size, uint8_t port )
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
        smtc_modem_request_extended_uplink( modem_stack_id, port, false, tx_frame_buffer, tx_frame_buffer_size,
                                            SMTC_MODEM_EXTENDED_UPLINK_ID_GNSS, &gnss_mw_tx_done_callback );
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

static void gnss_mw_send_event( gnss_mw_event_type_t event_type )
{
    /* The scan sequence ends when an event is sent to the application, except if SCAN_DONE */
    if( event_type != GNSS_MW_EVENT_SCAN_DONE )
    {
        task_running = false;
    }

    /* Increment the token on SCAN_DONE if the scan group is valid (and no aggregate) */
    if( event_type == GNSS_MW_EVENT_SCAN_DONE )
    {
        if( ( scan_aggregate == false ) && ( gnss_scan_group_queue_is_valid( &gnss_scan_group_queue ) ) )
        {
            gnss_scan_group_queue_increment_token( &gnss_scan_group_queue );
        }
    }

    /* Send the event to the application */
    pending_events = pending_events | ( 1 << event_type );
    MW_ASSERT_SMTC_MODEM_RC( smtc_modem_increment_event_middleware( SMTC_MODEM_EVENT_MIDDLEWARE_1, pending_events ) );
}

static void gnss_mw_set_solver_aiding_position( const uint8_t* payload, uint8_t size )
{
    /* Store the solver assistance position to be written to the LR11xx on the next scan */
    memcpy( solver_aiding_position_update, payload, SOLVER_AIDING_POSITION_SIZE );
    solver_aiding_position_update_received = true;

    /* We can switch to assisted scan for the next scan */
    current_scan_type = GNSS_MW_SCAN_TYPE_ASSISTED;
}

/* --- EOF ------------------------------------------------------------------ */