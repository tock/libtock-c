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

#include <stdio.h>
#include <stdint.h>

#include <libtock-sync/services/alarm.h>

#include <lr1110/lr1110.h>
#include <lr1110/us_915_ttn.h>

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

// included in lr1110 library

// #include "main_geolocation_wifi.h"
// #include "smtc_board.h"
// #include "smtc_hal.h"
// #include "apps_utilities.h"
// #include "apps_modem_common.h"
// #include "apps_modem_event.h"

// #include "wifi_middleware.h"
// #include "lr11xx_system.h"

// #include "smtc_modem_utilities.h"
// #include "smtc_board_ralf.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */


#define LORAWAN_REGION      SMTC_MODEM_REGION_US_915
#define LORAWAN_CLASS       SMTC_MODEM_CLASS_A

// wm1110dev parameters
#define LORAWAN_DEVICE_EUI  "70B3D57ED00650D9"
#define LORAWAN_JOIN_EUI    "901AB1F40E1BCC81"
#define LORAWAN_APP_KEY     "3356A7047ECF1F2F78C72AE9B1635BC1"



/*
 * -----------------------------------------------------------------------------
 * --- LoRaWAN Configuration ---------------------------------------------------
 */

/*!
 * @brief ADR custom list and retransmission parameters for EU868 / IN865 / RU864 / AU915 / CN470 /AS923 / KR920 regions
 */
#define CUSTOM_NB_TRANS_DR5_DR3 1
#define ADR_CUSTOM_LIST_DR5_DR3                        \
    {                                                  \
        5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 3, 3 \
    } /* 125kHz - SF7, SF8, SF9 */

/*!
 * @brief ADR custom list and retransmission parameters for US915 region
 */
#define CUSTOM_NB_TRANS_US915 2 // 1 in code, 2 on README
#define ADR_CUSTOM_LIST_US915                          \
    {                                                  \
        5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 3, 3 \
    } /* 125kHz - SF7, SF8, SF9 */

/*!
 * @brief ADR custom list and retransmission parameters for WW2G4 region
 */
#define CUSTOM_NB_TRANS_WW2G4 1
#define ADR_CUSTOM_LIST_WW2G4                          \
    {                                                  \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 \
    } /* SF12 */


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

static void on_modem_join_fail( void );

static void wifi_apps_modem_common_configure_lorawan_params( void );

/*!
 * @}
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

lr11xx_hal_context_t radio_context = {
    .nss    = LR1110_SPI_NSS_PIN,
    .busy   = LR1110_BUSY_PIN,
    .reset  = LR1110_NRESER_PIN,
    .spi_id = 3,
};

/**
 * @brief Main application entry point.
 */
int main( void )
{
    static apps_modem_event_callback_t smtc_event_callback = {
        .adr_mobile_to_static  = NULL,
        .alarm                 = on_modem_alarm,
        .almanac_update        = NULL,
        .down_data             = NULL,
        .join_fail             = on_modem_join_fail,
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

    lr11xx_system_clear_errors( &radio_context );

    /* Initialise the ralf_t object corresponding to the board */
    modem_radio = smtc_board_initialise_and_get_ralf( );

    /* Init board and peripherals */
    hal_mcu_init( );
    smtc_board_init_periph( );

    /* Init the Lora Basics Modem event callbacks */
    apps_modem_event_init( &smtc_event_callback );

    /* Init the modem and use smtc_event_process as event callback, please note that the callback will be called
     * immediately after the first call to modem_run_engine because of the reset detection */
    smtc_modem_init( modem_radio, &apps_modem_event_process );

    printf( "###### ===== LoRa Basics Modem Geolocation Wi-Fi example ==== ######\n\n" );
    apps_modem_common_display_lbm_version( );

    lr11xx_system_clear_errors( &radio_context );
    lr11xx_system_clear_irq_status(&radio_context, 0xFFFFFFFF);

    while( 1 )
    {
        lr11xx_system_clear_errors( &radio_context );

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
    printf("on_modem_reset %i\n", reset_count);

    /* Basic LoRaWAN configuration */
    wifi_apps_modem_common_configure_lorawan_params( );

    // We use TTN so only enable bank 2 channels.
    region_us_915_the_things_network_init();

    /* Start the Join process */
    ASSERT_SMTC_MODEM_RC( smtc_modem_join_network( stack_id ) );

    HAL_DBG_TRACE_INFO( "###### ===== JOINING ==== ######\n\n" );
}

static void on_modem_network_joined( void )
{
    printf("on_modem_network_joined successful!\n");

    mw_return_code_t wifi_rc;
    mw_version_t     mw_version;

    /* Set the custom ADR profile for geolocation scan & send */
    configure_adr( );

    /* Initialize Wi-Fi middleware */
    wifi_mw_get_version( &mw_version );
    // printf( "Initializing Wi-Fi middleware v%d.%d.%d\n", mw_version.major, mw_version.minor,
                        // mw_version.patch );
    wifi_mw_set_payload_format(WIFI_MW_PAYLOAD_MAC);
    wifi_mw_init( modem_radio, stack_id );

    ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( 5 ) );

    /* Start the Wi-Fi scan sequence */
    //smtc_modem_suspend_before_user_radio_access( );

    wifi_rc = wifi_mw_scan_start( 1 );
    if( wifi_rc != MW_RC_OK )
    {
        printf( "Error: Failed to start WiFi scan\n" );
    }
}

static void on_modem_alarm( void )
{
    //printf("on_modem_alarm\n");
    //wifi_mw_scan_rp_task_done();

    //ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( WIFI_SCAN_PERIOD ) );
    // printf( "smtc_modem_alarm_start_timer: %d s\n\n", WIFI_SCAN_PERIOD );

    // mw_return_code_t wifi_rc;
    // wifi_rc = wifi_mw_scan_start( 5 );
    // if( wifi_rc != MW_RC_OK )
    // {
    //     HAL_DBG_TRACE_ERROR( "Failed to start WiFi scan\n" );
    // }
}

static void on_modem_join_fail( void )
{
    printf("join failed!\n");
}

/*!
 * @brief User private function
 */

static void on_middleware_wifi_event( uint8_t pending_events )
{
    // printf("on_middleware_wifi_event\n");

    /* Parse events */
    if( wifi_mw_has_event( pending_events, WIFI_MW_EVENT_SCAN_DONE ) )
    {
        // printf( "Wi-Fi middleware event - SCAN DONE\n" );
        wifi_mw_get_event_data_scan_done( &wifi_results );
        wifi_mw_display_results( &wifi_results );
    }

    if( wifi_mw_has_event( pending_events, WIFI_MW_EVENT_TERMINATED ) )
    {
        wifi_mw_event_data_terminated_t event_data;

        // printf( "Wi-Fi middleware event - TERMINATED\n" );
        wifi_mw_get_event_data_terminated( &event_data );
        // HAL_DBG_TRACE_PRINTF( "TERMINATED info:\n" );
        printf( "-- number of scans sent: %u\n", event_data.nb_scans_sent );
    }

    if( wifi_mw_has_event( pending_events, WIFI_MW_EVENT_SCAN_CANCELLED ) )
    {
        printf( "Wi-Fi middleware event - SCAN CANCELLED\n" );
    }

    if( wifi_mw_has_event( pending_events, WIFI_MW_EVENT_ERROR_UNKNOWN ) )
    {
        printf( "Wi-Fi middleware event - UNEXPECTED ERROR\n" );
    }

    /* Program next scan */
    if( wifi_mw_has_event( pending_events, WIFI_MW_EVENT_ERROR_UNKNOWN ) ||
        wifi_mw_has_event( pending_events, WIFI_MW_EVENT_TERMINATED ) )
    {
        /* Program the next Wi-Fi group */
        // wifi_rc = wifi_mw_scan_start( WIFI_SCAN_PERIOD );
        //printf("receive unknown error\n");
        //smtc_modem_alarm_start_timer( 1 );
        // wifi_rc = wifi_mw_scan_start( 0 );
        // if( wifi_rc != MW_RC_OK )
        // {
        //     printf( "Error! Failed to start WiFi scan\n" );
        // }
    }

    wifi_mw_clear_pending_events( );
}

void configure_adr( void )
{
    // printf("configure_adr\n");

    smtc_modem_region_t region;

    ASSERT_SMTC_MODEM_RC( smtc_modem_get_region( stack_id, &region ) );

    // printf("Region number: %d\n", region);

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

static void wifi_apps_modem_common_configure_lorawan_params( void )
{
    smtc_modem_return_code_t rc = SMTC_MODEM_RC_OK;
    uint8_t dev_eui[8] = { 0 };
    uint8_t join_eui[8]  = { 0 };
    uint8_t app_key[16] = { 0 };

    hal_hex_to_bin((char*) LORAWAN_DEVICE_EUI, dev_eui, 8 );
    hal_hex_to_bin((char*) LORAWAN_JOIN_EUI, join_eui, 8 );
    hal_hex_to_bin((char*) LORAWAN_APP_KEY, app_key, 16 );

    rc = smtc_modem_set_deveui( stack_id, dev_eui );
    if( rc != SMTC_MODEM_RC_OK )
    {
        printf( "smtc_modem_set_deveui failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    rc = smtc_modem_set_joineui( stack_id, join_eui );
    if( rc != SMTC_MODEM_RC_OK )
    {
        printf( "smtc_modem_set_joineui failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    rc = smtc_modem_set_nwkkey( stack_id, app_key );
    if( rc != SMTC_MODEM_RC_OK )
    {
        printf( "smtc_modem_set_nwkkey failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    HAL_DBG_TRACE_INFO( "LoRaWAN parameters:\n" );

    rc = smtc_modem_get_deveui( stack_id, dev_eui );
    if( rc == SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_ARRAY( "DevEUI", dev_eui, SMTC_MODEM_EUI_LENGTH );
    }
    else
    {
        printf( "smtc_modem_get_deveui failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    rc = smtc_modem_get_joineui( stack_id, join_eui );
    if( rc == SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_ARRAY( "JoinEUI", join_eui, SMTC_MODEM_EUI_LENGTH );
    }
    else
    {
        printf( "smtc_modem_get_joineui failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    rc = smtc_modem_set_class( stack_id, LORAWAN_CLASS );
    if( rc != SMTC_MODEM_RC_OK )
    {
        printf( "smtc_modem_set_class failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    modem_class_to_string( LORAWAN_CLASS );

    rc = smtc_modem_set_region( stack_id, LORAWAN_REGION );
    if( rc != SMTC_MODEM_RC_OK )
    {
        printf( "smtc_modem_set_region failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    modem_region_to_string( LORAWAN_REGION );

    /* adapt the tx power offet depending on the board */
    rc |= smtc_modem_set_tx_power_offset_db( stack_id, smtc_board_get_tx_power_offset( ) );
}

/*!
 * @}
 */

/* --- EOF ------------------------------------------------------------------ */
