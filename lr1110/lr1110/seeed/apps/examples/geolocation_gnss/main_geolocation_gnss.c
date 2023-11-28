/*!
 * @ingroup   apps_geolocation
 * @file      main_geolocation_gnss.c
 *
 * @brief     LoRa Basics Modem LR11XX Geolocation GNSS example with *scan group" for static geolocation.
 *
 * @copyright
 * @parblock
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
 * @endparblock
 */

/*!
 * @addtogroup apps_geolocation
 * LoRa Basics Modem LR11XX Geolocation GNSS example for static objects
 * @{
 */

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include "main_geolocation_gnss.h"
#include "smtc_board.h"
#include "smtc_hal.h"
#include "apps_utilities.h"
#include "apps_modem_common.h"
#include "apps_modem_event.h"

#include "gnss_middleware.h"
#include "lr11xx_system.h"

#include "smtc_modem_utilities.h"
#include "smtc_board_ralf.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */
/**
 * @brief LR11XX radio firmware
 */
#define LR1110_FW_VERSION 0x0307
#define LR1110_FW_TYPE 0x01
#define LR1120_FW_VERSION 0x0101
#define LR1120_FW_TYPE 0x02

/**
 * @brief Duration in second after last ALC sync response received to consider the local clock time invalid
 *
 * Set time valid for 1 day (to be fine tuned depending on board properties)
 */
#define APP_ALC_TIMING_INVALID ( 3600 * 24 )

/**
 * @brief Interval in second between two consecutive ALC sync requests
 *
 * 3 time sync requests per day
 */
#define APP_ALC_TIMING_INTERVAL ( APP_ALC_TIMING_INVALID / 3 )

/**
 * @brief Duration in second after Join to start the application keep alive sequence
 */
#define APP_TIMER_START ( 10 )

/**
 * @brief Duration in second for keep alive alarm sequence
 */
#define APP_TIMER_KEEP_ALIVE ( 30 )

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*!
 * @brief Stack identifier
 */
static uint8_t stack_id = 0;

/*!
 * @brief Modem radio
 */
static ralf_t* modem_radio;

/*!
 * @brief First time sync status for application startup
 */
static bool is_first_time_sync = true;

/*!
 * @brief ADR custom list and retransmission definition for EU868 / IN865 / RU864 / AU915 / CN470 /AS923 / KR920 regions
 */
static const uint8_t adr_custom_list_dr5_dr3[16] = ADR_CUSTOM_LIST_DR5_DR3;
static const uint8_t custom_nb_trans_dr5_dr3     = CUSTOM_NB_TRANS_DR5_DR3;

/*!
 * @brief ADR custom list and retransmission definition for US915 region
 */
static const uint8_t adr_custom_list_us915[16] = ADR_CUSTOM_LIST_US915;
static const uint8_t custom_nb_trans_us915     = CUSTOM_NB_TRANS_US915;

/*!
 * @brief ADR custom list and retransmission definition for WW2G4 region
 */
static const uint8_t adr_custom_list_ww2g4[16] = ADR_CUSTOM_LIST_WW2G4;
static const uint8_t custom_nb_trans_ww2g4     = CUSTOM_NB_TRANS_WW2G4;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*!
 * @brief Helper function that configure the custom ADR configuration for geolocation scan & send, based on the region
 * already configured in the stack.
 *
 * Prior using this function, the region must have been set already in the stack.
 */
static void configure_adr( void );

/*!
 * @addtogroup basics_modem_evt_callback
 * LoRa Basics Modem event callbacks
 * @{
 */

/*!
 * @brief Reset event callback
 *
 * @param [in] reset_count reset counter from the modem
 */
static void on_modem_reset( uint16_t reset_count );

/*!
 * @brief Network Joined event callback
 */
static void on_modem_network_joined( void );

/*!
 * @brief Alarm event callback
 */
static void on_modem_alarm( void );

/*!
 * @brief Clock synchronisation event callback
 */
static void on_modem_clk_synch( smtc_modem_event_time_status_t time_status );

/*!
 * @brief Downlink data event callback.
 *
 * @param [in] rssi       RSSI in signed value in dBm + 64
 * @param [in] snr        SNR signed value in 0.25 dB steps
 * @param [in] rx_window  RX window
 * @param [in] port       LoRaWAN port
 * @param [in] payload    Received buffer pointer
 * @param [in] size       Received buffer size
 */
static void on_modem_down_data( int8_t rssi, int8_t snr, smtc_modem_event_downdata_window_t rx_window, uint8_t port,
                                const uint8_t* payload, uint8_t size );

/*!
 * @brief GNSS Almanac update event callback
 */
static void on_modem_almanac_update( smtc_modem_event_almanac_update_status_t status );

/*!
 * @brief GNSS middleware event callback
 */
static void on_middleware_gnss_event( uint8_t pending_events );

/*!
 * @}
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

/**
 * @brief Main application entry point.
 */
int main( void )
{
    lr11xx_system_version_t            lr11xx_fw_version;
    lr11xx_status_t                    status;
    static apps_modem_event_callback_t smtc_event_callback = {
        .adr_mobile_to_static  = NULL,
        .alarm                 = on_modem_alarm,
        .almanac_update        = on_modem_almanac_update,
        .down_data             = on_modem_down_data,
        .join_fail             = NULL,
        .joined                = on_modem_network_joined,
        .link_status           = NULL,
        .mute                  = NULL,
        .new_link_adr          = NULL,
        .reset                 = on_modem_reset,
        .set_conf              = NULL,
        .stream_done           = NULL,
        .time_updated_alc_sync = on_modem_clk_synch,
        .tx_done               = NULL,
        .upload_done           = NULL,
        .user_radio_access     = NULL,
        .middleware_1          = on_middleware_gnss_event,
        .middleware_2          = NULL,
    };

    /* Initialise the ralf_t object corresponding to the board */
    modem_radio = smtc_board_initialise_and_get_ralf( );

    /* Disable IRQ to avoid unwanted behaviour during init */
    hal_mcu_disable_irq( );

    /* Init board and peripherals */
    hal_mcu_init( );
    smtc_board_init_periph( );

    /* Init the Lora Basics Modem event callbacks */
    apps_modem_event_init( &smtc_event_callback );

    /* Init the modem and use apps_modem_event_process as event callback, please note that the callback will be called
     * immediately after the first call to modem_run_engine because of the reset detection */
    smtc_modem_init( modem_radio, &apps_modem_event_process );

    /* Re-enable IRQ */
    hal_mcu_enable_irq( );

    HAL_DBG_TRACE_MSG( "\n" );
    HAL_DBG_TRACE_INFO(
        "###### ===== LoRa Basics Modem GNSS Geolocation example (for static objects) ==== ######\n\n" );

    apps_modem_common_display_lbm_version( );

    /* Check LR11XX Firmware version */
    ASSERT_SMTC_MODEM_RC( smtc_modem_suspend_before_user_radio_access( ) ); /* protect from radio access conflicts */
    status = lr11xx_system_get_version( modem_radio->ral.context, &lr11xx_fw_version );
    ASSERT_SMTC_MODEM_RC( smtc_modem_resume_after_user_radio_access( ) );
    if( status != LR11XX_STATUS_OK )
    {
        HAL_DBG_TRACE_ERROR( "Failed to get LR11XX firmware version\n" );
        mcu_panic( );
    }
    if( ( ( lr11xx_fw_version.fw != LR1110_FW_VERSION ) && ( lr11xx_fw_version.type = LR1110_FW_TYPE ) ) &&
        ( ( lr11xx_fw_version.fw != LR1120_FW_VERSION ) && ( lr11xx_fw_version.type = LR1120_FW_TYPE ) ) )
    {
        HAL_DBG_TRACE_ERROR( "Wrong LR11XX firmware version, expected 0x%04X, got 0x%04X\n", LR1110_FW_VERSION,
                             lr11xx_fw_version.fw );
        mcu_panic( );
    }
    HAL_DBG_TRACE_INFO( "LR11XX FW   : 0x%04X\n", lr11xx_fw_version.fw );

    while( 1 )
    {
        /* Execute modem runtime, this function must be called again in sleep_time_ms milliseconds or sooner. */
        uint32_t sleep_time_ms = smtc_modem_run_engine( );
        /* go in low power */
        hal_mcu_set_sleep_for_ms( sleep_time_ms );
    }
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/*!
 * @brief LoRa Basics Modem event callbacks called by smtc_event_process function
 */

static void on_modem_reset( uint16_t reset_count )
{
    apps_modem_common_configure_lorawan_params( stack_id );

    /* Configure modem DM status for regular almanac status update */
    smtc_modem_dm_info_interval_format_t format   = SMTC_MODEM_DM_INFO_INTERVAL_IN_DAY;
    uint8_t                              interval = 1;
    ASSERT_SMTC_MODEM_RC( smtc_modem_dm_set_info_interval( format, interval ) );

    /* Active almanac update OTA - WARNING: will remove all other DM message */
    uint8_t info_field = SMTC_MODEM_DM_FIELD_ALMANAC_STATUS;
    ASSERT_SMTC_MODEM_RC( smtc_modem_dm_set_info_fields( &info_field, 1 ) );

    /* Start the Join process */
    ASSERT_SMTC_MODEM_RC( smtc_modem_join_network( stack_id ) );

    HAL_DBG_TRACE_INFO( "###### ===== JOINING ==== ######\n\n" );
}

static void on_modem_network_joined( void )
{
    /* Start time sync (ALC sync), necessary for GNSS scan:
    The interval_s indicates how often the LBM will request a time sync from the DAS.
    If no time sync downlink has been received from the DAS after the invalid_delay_s is elapsed,
    the LBM will report SMTC_MODEM_RC_NO_TIME on smtc_modem_get_time() call. */
    /* -- */
    ASSERT_SMTC_MODEM_RC( smtc_modem_time_set_sync_interval_s( APP_ALC_TIMING_INTERVAL ) );     /* keep call order */
    ASSERT_SMTC_MODEM_RC( smtc_modem_time_set_sync_invalid_delay_s( APP_ALC_TIMING_INVALID ) ); /* keep call order */
    /* Start the service */
    ASSERT_SMTC_MODEM_RC( smtc_modem_time_start_sync_service( stack_id, SMTC_MODEM_TIME_ALC_SYNC ) );
}

static void on_modem_alarm( void )
{
    ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( GNSS_SCAN_GROUP_PERIOD_DEFAULT ) );
    HAL_DBG_TRACE_PRINTF( "smtc_modem_alarm_start_timer: %d s\n\n", GNSS_SCAN_GROUP_PERIOD_DEFAULT );

    // mw_return_code_t gnss_rc;
    // gnss_rc = gnss_mw_scan_start( GNSS_SCAN_MODE, 0 ); /* start ASAP */
    // if( gnss_rc != MW_RC_OK )
    // {
    //     HAL_DBG_TRACE_ERROR( "Failed to start GNSS scan\n" );
    // }
}

static void on_modem_clk_synch( smtc_modem_event_time_status_t time_status )
{
    mw_return_code_t gnss_rc;
    mw_version_t     mw_version;

    if( time_status != SMTC_MODEM_EVENT_TIME_NOT_VALID )
    {
        if( is_first_time_sync == true )
        {
            /* Set the custom ADR profile for geolocation scan & send */
            configure_adr( );

            /* Initialize GNSS middleware */
            gnss_mw_get_version( &mw_version );
            HAL_DBG_TRACE_INFO( "Initializing GNSS middleware v%d.%d.%d\n", mw_version.major, mw_version.minor,
                                mw_version.patch );
            gnss_mw_init( modem_radio, stack_id );
            gnss_mw_set_constellations( GNSS_MW_CONSTELLATION_GPS_BEIDOU );

            /* Set user defined assistance position */
#if MODEM_EXAMPLE_ASSISTANCE_POSITION_AUTO == false
            const char* assistance_position_text = MODEM_EXAMPLE_ASSISTANCE_POSITION_TEXT;
            gnss_mw_set_user_aiding_position( MODEM_EXAMPLE_ASSISTANCE_POSITION_LAT,
                                              MODEM_EXAMPLE_ASSISTANCE_POSITION_LONG );
            HAL_DBG_TRACE_WARNING( "User defined assistance position has been set in %s\n", assistance_position_text );
#endif

            ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( 5 ) );

            /* Start the GNSS scan group sequence */
            gnss_rc = gnss_mw_scan_start( GNSS_SCAN_MODE, 0 ); /* start ASAP */
            if( gnss_rc != MW_RC_OK )
            {
                HAL_DBG_TRACE_ERROR( "Failed to start GNSS scan\n" );
            }
        }

        is_first_time_sync = false;
    }
}

static void on_modem_down_data( int8_t rssi, int8_t snr, smtc_modem_event_downdata_window_t rx_window, uint8_t port,
                                const uint8_t* payload, uint8_t size )
{
    HAL_DBG_TRACE_INFO( "Downlink received:\n" );
    HAL_DBG_TRACE_INFO( "  - LoRaWAN Fport = %d\n", port );
    HAL_DBG_TRACE_INFO( "  - Payload size  = %d\n", size );
    HAL_DBG_TRACE_INFO( "  - RSSI          = %d dBm\n", rssi - 64 );
    HAL_DBG_TRACE_INFO( "  - SNR           = %d dB\n", snr >> 2 );

    if( size != 0 )
    {
        HAL_DBG_TRACE_ARRAY( "Payload", payload, size );

        /* Forward downlink to GNSS middleware to handle it if necessary */
        gnss_mw_handle_downlink( port, payload, size );
    }
}

static void on_modem_almanac_update( smtc_modem_event_almanac_update_status_t status )
{
    if( status == SMTC_MODEM_EVENT_ALMANAC_UPDATE_STATUS_REQUESTED )
    {
        HAL_DBG_TRACE_INFO( "Almanac update is not completed: sending new request\n" );
#if 0
        uint8_t dm_almanac_status = SMTC_MODEM_DM_FIELD_ALMANAC_STATUS;
        ASSERT_SMTC_MODEM_RC( smtc_modem_dm_request_single_uplink( &dm_almanac_status, 1 ) );
#endif
    }
    else
    {
        HAL_DBG_TRACE_INFO( "Almanac update is completed\n" );
    }
}

static void on_middleware_gnss_event( uint8_t pending_events )
{
    mw_return_code_t gnss_rc;

    /* Parse events */
    if( gnss_mw_has_event( pending_events, GNSS_MW_EVENT_SCAN_DONE ) )
    {
        gnss_mw_event_data_scan_done_t event_data;

        HAL_DBG_TRACE_INFO( "GNSS middleware event - SCAN DONE\n" );
        gnss_mw_get_event_data_scan_done( &event_data );
        gnss_mw_display_results( &event_data );

        if( event_data.context.almanac_update_required )
        {
            uint8_t dm_almanac_status = SMTC_MODEM_DM_FIELD_ALMANAC_STATUS;
            ASSERT_SMTC_MODEM_RC( smtc_modem_dm_request_single_uplink( &dm_almanac_status, 1 ) );
        }
    }

    if( gnss_mw_has_event( pending_events, GNSS_MW_EVENT_TERMINATED ) )
    {
        gnss_mw_event_data_terminated_t event_data;

        HAL_DBG_TRACE_INFO( "GNSS middleware event - TERMINATED\n" );
        gnss_mw_get_event_data_terminated( &event_data );
        HAL_DBG_TRACE_PRINTF( "TERMINATED info:\n" );
        HAL_DBG_TRACE_PRINTF( "-- number of scans sent: %u\n", event_data.nb_scans_sent );
        HAL_DBG_TRACE_PRINTF( "-- aiding position check sent: %d\n", event_data.aiding_position_check_sent );
        HAL_DBG_TRACE_PRINTF( "-- indoor detected: %d\n", event_data.indoor_detected );
    }

    if( gnss_mw_has_event( pending_events, GNSS_MW_EVENT_SCAN_CANCELLED ) )
    {
        HAL_DBG_TRACE_INFO( "GNSS middleware event - SCAN CANCELLED\n" );
    }

    if( gnss_mw_has_event( pending_events, GNSS_MW_EVENT_ERROR_NO_TIME ) )
    {
        HAL_DBG_TRACE_ERROR( "GNSS middleware event - ERROR NO TIME\n" );
        ASSERT_SMTC_MODEM_RC( smtc_modem_time_trigger_sync_request( stack_id ) );
    }

    if( gnss_mw_has_event( pending_events, GNSS_MW_EVENT_ERROR_ALMANAC_UPDATE ) )
    {
        HAL_DBG_TRACE_ERROR( "GNSS middleware event - ALMANAC UPDATE REQUIRED\n" );
        uint8_t dm_almanac_status = SMTC_MODEM_DM_FIELD_ALMANAC_STATUS;
        ASSERT_SMTC_MODEM_RC( smtc_modem_dm_request_single_uplink( &dm_almanac_status, 1 ) );
    }

    if( gnss_mw_has_event( pending_events, GNSS_MW_EVENT_ERROR_NO_AIDING_POSITION ) )
    {
        HAL_DBG_TRACE_ERROR( "GNSS middleware event - ERROR NO AIDING POSITION set\n" );

/* Set user defined assistance position */
#if MODEM_EXAMPLE_ASSISTANCE_POSITION_AUTO == false
        const char* assistance_position_text = MODEM_EXAMPLE_ASSISTANCE_POSITION_TEXT;
        gnss_mw_set_user_aiding_position( MODEM_EXAMPLE_ASSISTANCE_POSITION_LAT,
                                          MODEM_EXAMPLE_ASSISTANCE_POSITION_LONG );
        HAL_DBG_TRACE_WARNING( "User defined assistance position has been set in %s\n", assistance_position_text );
#endif
    }

    if( gnss_mw_has_event( pending_events, GNSS_MW_EVENT_ERROR_UNKNOWN ) )
    {
        HAL_DBG_TRACE_ERROR( "GNSS middleware event - UNEXPECTED ERROR\n" );
    }

    /* Program the next GNSS scan group if event != SCAN_DONE */
    if( gnss_mw_has_event( pending_events, GNSS_MW_EVENT_TERMINATED ) ||
        gnss_mw_has_event( pending_events, GNSS_MW_EVENT_ERROR_NO_TIME ) ||
        gnss_mw_has_event( pending_events, GNSS_MW_EVENT_ERROR_ALMANAC_UPDATE ) ||
        gnss_mw_has_event( pending_events, GNSS_MW_EVENT_ERROR_NO_AIDING_POSITION ) ||
        gnss_mw_has_event( pending_events, GNSS_MW_EVENT_ERROR_UNKNOWN ) ||
        gnss_mw_has_event( pending_events, GNSS_MW_EVENT_SCAN_CANCELLED ) )
    {
        gnss_rc = gnss_mw_scan_start( GNSS_SCAN_MODE, GNSS_SCAN_GROUP_PERIOD );
        if( gnss_rc != MW_RC_OK )
        {
            HAL_DBG_TRACE_ERROR( "Failed to start GNSS scan\n" );
        }
    }

    gnss_mw_clear_pending_events( );
}

void configure_adr( void )
{
    smtc_modem_region_t region;

    ASSERT_SMTC_MODEM_RC( smtc_modem_get_region( stack_id, &region ) );

    /* Set the ADR profile once joined */
    switch( region )
    {
    case SMTC_MODEM_REGION_EU_868:
    case SMTC_MODEM_REGION_IN_865:
    case SMTC_MODEM_REGION_RU_864:
    case SMTC_MODEM_REGION_AU_915:
    case SMTC_MODEM_REGION_AS_923_GRP1:
    case SMTC_MODEM_REGION_AS_923_GRP2:
    case SMTC_MODEM_REGION_AS_923_GRP3:
    case SMTC_MODEM_REGION_CN_470:
    case SMTC_MODEM_REGION_CN_470_RP_1_0:
    case SMTC_MODEM_REGION_KR_920:
        ASSERT_SMTC_MODEM_RC(
            smtc_modem_adr_set_profile( stack_id, SMTC_MODEM_ADR_PROFILE_CUSTOM, adr_custom_list_dr5_dr3 ) );
        ASSERT_SMTC_MODEM_RC( smtc_modem_set_nb_trans( stack_id, custom_nb_trans_dr5_dr3 ) );
        break;
    case SMTC_MODEM_REGION_US_915:
        ASSERT_SMTC_MODEM_RC(
            smtc_modem_adr_set_profile( stack_id, SMTC_MODEM_ADR_PROFILE_CUSTOM, adr_custom_list_us915 ) );
        ASSERT_SMTC_MODEM_RC( smtc_modem_set_nb_trans( stack_id, custom_nb_trans_us915 ) );
        break;
    case SMTC_MODEM_REGION_WW2G4:
        ASSERT_SMTC_MODEM_RC(
            smtc_modem_adr_set_profile( stack_id, SMTC_MODEM_ADR_PROFILE_CUSTOM, adr_custom_list_ww2g4 ) );
        ASSERT_SMTC_MODEM_RC( smtc_modem_set_nb_trans( stack_id, custom_nb_trans_ww2g4 ) );
        break;
    default:
        HAL_DBG_TRACE_ERROR( "Region not supported in this example, could not set custom ADR profile\n" );
        break;
    }

    /* Disable auto switch to network controlled after a certain amount of TX without RX */
    ASSERT_SMTC_MODEM_RC( smtc_modem_connection_timeout_set_thresholds( stack_id, 0, 0 ) );
}

/*!
 * @}
 */

/* --- EOF ------------------------------------------------------------------ */
