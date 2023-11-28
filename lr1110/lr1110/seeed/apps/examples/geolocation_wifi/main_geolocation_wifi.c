/*!
 * @ingroup   apps_geolocation
 * @file      main_geolocation_wifi.c
 *
 * @brief     LoRa Basics Modem LR11XX Geolocation Wi-Fi example
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
 * LoRa Basics Modem LR11XX Geolocation Wi-Fi example
 * @{
 */

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include "main_geolocation_wifi.h"
#include "smtc_board.h"
#include "smtc_hal.h"
#include "apps_utilities.h"
#include "apps_modem_common.h"
#include "apps_modem_event.h"

#include "wifi_middleware.h"
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
 * @brief Wi-Fi output results
 */
static wifi_mw_event_data_scan_done_t wifi_results;

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
 * @brief Wi-Fi middleware event callback
 */
static void on_middleware_wifi_event( uint8_t pending_events );

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
        .almanac_update        = NULL,
        .down_data             = NULL,
        .join_fail             = NULL,
        .joined                = on_modem_network_joined,
        .link_status           = NULL,
        .mute                  = NULL,
        .new_link_adr          = NULL,
        .reset                 = on_modem_reset,
        .set_conf              = NULL,
        .stream_done           = NULL,
        .time_updated_alc_sync = NULL,
        .tx_done               = NULL,
        .upload_done           = NULL,
        .user_radio_access     = NULL,
        .middleware_1          = NULL,
        .middleware_2          = on_middleware_wifi_event,
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

    /* Init the modem and use smtc_event_process as event callback, please note that the callback will be called
     * immediately after the first call to modem_run_engine because of the reset detection */
    smtc_modem_init( modem_radio, &apps_modem_event_process );

    /* Re-enable IRQ */
    hal_mcu_enable_irq( );

    HAL_DBG_TRACE_MSG( "\n" );
    HAL_DBG_TRACE_INFO( "###### ===== LoRa Basics Modem Geolocation Wi-Fi example ==== ######\n\n" );
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
    /* Basic LoRaWAN configuration */
    apps_modem_common_configure_lorawan_params( stack_id );

    /* Start the Join process */
    ASSERT_SMTC_MODEM_RC( smtc_modem_join_network( stack_id ) );

    HAL_DBG_TRACE_INFO( "###### ===== JOINING ==== ######\n\n" );
}

static void on_modem_network_joined( void )
{
    mw_return_code_t wifi_rc;
    mw_version_t     mw_version;

    /* Set the custom ADR profile for geolocation scan & send */
    configure_adr( );

    /* Initialize Wi-Fi middleware */
    wifi_mw_get_version( &mw_version );
    HAL_DBG_TRACE_INFO( "Initializing Wi-Fi middleware v%d.%d.%d\n", mw_version.major, mw_version.minor,
                        mw_version.patch );
    wifi_mw_init( modem_radio, stack_id );

    ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( 5 ) );

    /* Start the Wi-Fi scan sequence */
    wifi_rc = wifi_mw_scan_start( 0 );
    if( wifi_rc != MW_RC_OK )
    {
        HAL_DBG_TRACE_ERROR( "Failed to start WiFi scan\n" );
    }
}

static void on_modem_alarm( void )
{
    ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( WIFI_SCAN_PERIOD ) );
    HAL_DBG_TRACE_PRINTF( "smtc_modem_alarm_start_timer: %d s\n\n", WIFI_SCAN_PERIOD );

    // mw_return_code_t wifi_rc;
    // wifi_rc = wifi_mw_scan_start( 0 );
    // if( wifi_rc != MW_RC_OK )
    // {
    //     HAL_DBG_TRACE_ERROR( "Failed to start WiFi scan\n" );
    // }
}

/*!
 * @brief User private function
 */

static void on_middleware_wifi_event( uint8_t pending_events )
{
    mw_return_code_t wifi_rc;

    /* Parse events */
    if( wifi_mw_has_event( pending_events, WIFI_MW_EVENT_SCAN_DONE ) )
    {
        HAL_DBG_TRACE_INFO( "Wi-Fi middleware event - SCAN DONE\n" );
        wifi_mw_get_event_data_scan_done( &wifi_results );
        wifi_mw_display_results( &wifi_results );
    }

    if( wifi_mw_has_event( pending_events, WIFI_MW_EVENT_TERMINATED ) )
    {
        wifi_mw_event_data_terminated_t event_data;

        HAL_DBG_TRACE_INFO( "Wi-Fi middleware event - TERMINATED\n" );
        wifi_mw_get_event_data_terminated( &event_data );
        HAL_DBG_TRACE_PRINTF( "TERMINATED info:\n" );
        HAL_DBG_TRACE_PRINTF( "-- number of scans sent: %u\n", event_data.nb_scans_sent );
    }

    if( wifi_mw_has_event( pending_events, WIFI_MW_EVENT_SCAN_CANCELLED ) )
    {
        HAL_DBG_TRACE_INFO( "Wi-Fi middleware event - SCAN CANCELLED\n" );
    }

    if( wifi_mw_has_event( pending_events, WIFI_MW_EVENT_ERROR_UNKNOWN ) )
    {
        HAL_DBG_TRACE_INFO( "Wi-Fi middleware event - UNEXPECTED ERROR\n" );
    }

    /* Program next scan */
    if( wifi_mw_has_event( pending_events, WIFI_MW_EVENT_ERROR_UNKNOWN ) ||
        wifi_mw_has_event( pending_events, WIFI_MW_EVENT_TERMINATED ) )
    {
        /* Program the next Wi-Fi group */
        wifi_rc = wifi_mw_scan_start( WIFI_SCAN_PERIOD );
        if( wifi_rc != MW_RC_OK )
        {
            HAL_DBG_TRACE_ERROR( "Failed to start WiFi scan\n" );
        }
    }

    wifi_mw_clear_pending_events( );
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
