/*!
 * @file      main_lorawan.c
 *
 * @brief     LoRa Basics Modem Class A/C device implementation
 *
 * @copyright
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

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

// #include "main_lorawan.h"
// #include "lorawan_key_config.h"
// #include "smtc_board.h"
// #include "smtc_hal.h"
// #include "apps_modem_common.h"
// #include "apps_modem_event.h"
// #include "smtc_modem_api.h"
// #include "device_management_defs.h"
// #include "smtc_board_ralf.h"
// #include "apps_utilities.h"
// #include "smtc_modem_utilities.h"

#include <lr1110/lr1110.h>
// #include <timer.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <humidity.h>
#include <temperature.h>
#include <timer.h>

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*!
 * @brief Stringify constants
 */
#define xstr( a ) str( a )
#define str( a ) #a

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
 * @brief User application data
 */
static uint8_t app_data_buffer[LORAWAN_APP_DATA_MAX_SIZE];

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*!
 * @brief   Send an application frame on LoRaWAN port defined by LORAWAN_APP_PORT
 *
 * @param [in] buffer     Buffer containing the LoRaWAN buffer
 * @param [in] length     Payload length
 * @param [in] confirmed  Send a confirmed or unconfirmed uplink [false : unconfirmed / true : confirmed]
 */
static void send_frame( const uint8_t* buffer, const uint8_t length, const bool confirmed );

/*!
 * @brief Parse the received downlink
 *
 * @remark Demonstrates how a TLV-encoded command sequence received by downlink can control the state of an LED. It can
 * easily be extended to handle other commands received on the same port or another port.
 *
 * @param [in] port LoRaWAN port
 * @param [in] payload Payload Buffer
 * @param [in] size Payload size
 */
static void parse_downlink_frame( uint8_t port, const uint8_t* payload, uint8_t size );

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
 * @brief Tx done event callback
 *
 * @param [in] status tx done status @ref smtc_modem_event_txdone_status_t
 */
static void on_modem_tx_done( smtc_modem_event_txdone_status_t status );

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

static void on_modem_join_fail( void );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

/**
 * @brief Main application entry point.
 */

lr11xx_hal_context_t radio_context = {
    .nss    = LR1110_SPI_NSS_PIN,
    .busy   = LR1110_BUSY_PIN,
    .reset  = LR1110_NRESER_PIN,
    .spi_id = 3,
};

int main( void )
{
    //hal_gpio_init_out( LR1110_SPI_NSS_PIN, HAL_GPIO_SET );
    // hal_gpio_init_in( LR1110_BUSY_PIN, HAL_GPIO_PULL_MODE_NONE, HAL_GPIO_IRQ_MODE_OFF, NULL );
    // hal_gpio_init_in( LR1110_IRQ_PIN, HAL_GPIO_PULL_MODE_DOWN, HAL_GPIO_IRQ_MODE_RISING, NULL );
    // hal_gpio_init_out( LR1110_NRESER_PIN, HAL_GPIO_SET );

    // lr11xx_system_version_t version;
    // lr11xx_status_t status;

    // hal_spi_init( );

    // printf("\nafter spi init\n");

    // lr11xx_hal_reset(&radio_context);

    lr11xx_system_clear_errors( &radio_context );

    lr11xx_status_t status;
    lr11xx_system_version_t version;
    status = lr11xx_system_get_version( &radio_context, &version );
    printf("Hardware Version: %u, 0x%04X\n", version.hw, version.hw);
    printf("Type: %u, 0x%04X\n", version.type, version.type);
    printf("Firmware Version: %u, 0x%04X\n", version.fw, version.fw);
    // if( status != LR11XX_STATUS_OK )
    // {
    //     printf( "Failed to get LR11XX firmware version\n" );
    // }
    // if( ( ( version.fw != LR1110_FW_VERSION ) && ( version.type = LR1110_FW_TYPE ) ) &&
    //     ( ( version.fw != LR1120_FW_VERSION ) && ( version.type = LR1120_FW_TYPE ) ) )
    // {
    //     printf( "Wrong LR11XX firmware version, expected 0x%04X, got 0x%04X\n", LR1110_FW_VERSION,
    //                          version.fw );
    // }

    lr11xx_system_uid_t unique_identifier;
    status= lr11xx_system_read_uid( &radio_context, &unique_identifier );

    printf("uid %x %x %x\n", unique_identifier[0], unique_identifier[1], unique_identifier[2]);


    printf("start of lorawan app\n");

    static apps_modem_event_callback_t smtc_event_callback = {
        .adr_mobile_to_static  = NULL,
        .alarm                 = on_modem_alarm,
        .almanac_update        = NULL,
        .down_data             = on_modem_down_data,
        .join_fail             = on_modem_join_fail,
        .joined                = on_modem_network_joined,
        .link_status           = NULL,
        .mute                  = NULL,
        .new_link_adr          = NULL,
        .reset                 = on_modem_reset,
        .set_conf              = NULL,
        .stream_done           = NULL,
        .time_updated_alc_sync = NULL,
        .tx_done               = on_modem_tx_done,
        .upload_done           = NULL,
    };

    /* Initialise the ralf_t object corresponding to the board */
    ralf_t* modem_radio = smtc_board_initialise_and_get_ralf( );

    /* Disable IRQ to avoid unwanted behaviour during init */
    //hal_mcu_disable_irq( );


    /* Init board and peripherals */
    hal_mcu_init( );
    smtc_board_init_periph( );

    /* Init the Lora Basics Modem event callbacks */
    apps_modem_event_init( &smtc_event_callback );

    /* Init the modem and use apps_modem_event_process as event callback, please note that the callback will be called
     * immediately after the first call to modem_run_engine because of the reset detection */
    smtc_modem_init( modem_radio, &apps_modem_event_process ); // cause process fault

    /* Re-enable IRQ */
    // hal_mcu_enable_irq( );

    //HAL_DBG_TRACE_MSG( "\n" );
    //HAL_DBG_TRACE_INFO( "###### ===== LoRa Basics Modem LoRaWAN Class A/C demo application ==== ######\n\n" );
    printf("\n###### ===== LoRa Basics Modem LoRaWAN Class A/C demo application ==== ######\n\n");

    /* LoRa Basics Modem Version */
    apps_modem_common_display_lbm_version( );

    /* Configure the partial low power mode */
    // hal_mcu_partial_sleep_enable( APP_PARTIAL_SLEEP ); // smtc function implementation is empty

    lr11xx_system_clear_errors( &radio_context );
    lr11xx_system_clear_irq_status(&radio_context, 0xFFFFFFFF);

    printf("inside loop\n");
    
    while( 1 )
    {
        lr11xx_system_clear_errors( &radio_context );
        
        // delay_ms(10);
        /* Execute modem runtime, this function must be called again in sleep_time_ms milliseconds or sooner. */
        uint32_t sleep_time_ms = smtc_modem_run_engine( ); // cause process fault

        // printf("sleep %i\n", sleep_time_ms);

        /* go in low power */
        hal_mcu_set_sleep_for_ms( sleep_time_ms );

        // delay_ms(1000);
    }
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

static void on_modem_reset( uint16_t reset_count )
{
    printf("on_modem_reset\n");

    HAL_DBG_TRACE_INFO( "Application parameters:\n" );
    HAL_DBG_TRACE_INFO( "  - LoRaWAN uplink Fport = %d\n", LORAWAN_APP_PORT );
    HAL_DBG_TRACE_INFO( "  - DM report interval   = %d\n", APP_TX_DUTYCYCLE );
    HAL_DBG_TRACE_INFO( "  - Confirmed uplink     = %s\n", ( LORAWAN_CONFIRMED_MSG_ON == true ) ? "Yes" : "No" );

    apps_modem_common_configure_lorawan_params( stack_id );

    ASSERT_SMTC_MODEM_RC( smtc_modem_join_network( stack_id ) );
}

static void on_modem_network_joined( void )
{
    printf("on_modem_network_joined successful!\n");

    ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( APP_TX_DUTYCYCLE ) );

    ASSERT_SMTC_MODEM_RC( smtc_modem_adr_set_profile( stack_id, LORAWAN_DEFAULT_DATARATE, adr_custom_list ) );
}

static void on_modem_join_fail( void )
{
    printf("join failed!\n");
}

static void on_modem_alarm( void )
{
    printf("on_modem_alarm\n");
    smtc_modem_status_mask_t modem_status;
    // uint32_t                 charge        = 0;
    uint8_t                  app_data_size = 0;

    /* Schedule next packet transmission */
    ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( APP_TX_DUTYCYCLE ) );
    HAL_DBG_TRACE_PRINTF( "smtc_modem_alarm_start_timer: %d s\n\n", APP_TX_DUTYCYCLE );

    ASSERT_SMTC_MODEM_RC( smtc_modem_get_status( stack_id, &modem_status ) );
    modem_status_to_string( modem_status );

    // ASSERT_SMTC_MODEM_RC( smtc_modem_get_charge( &charge ) );

    printf("[Sensors] Sampling Temperature and Humidity sensors once.\n");

    bool temperature_available    = driver_exists(DRIVER_NUM_TEMPERATURE);
    bool humidity_available       = driver_exists(DRIVER_NUM_HUMIDITY);
    int temp = 0;
    unsigned humi = 0;
    if (temperature_available) {
        temperature_read_sync(&temp);
        printf("Temperature: %d.%d deg C, send %d to TTN\n", temp / 100, temp % 100, temp);
    } else {
        printf("Temperature sensor not available.\n");
    }

    if (humidity_available) {
        humidity_read_sync(&humi);
        printf("Humidity: %d.%d%%, send %d to TTN\n", humi / 100, humi % 100, humi);
    } else {
        printf("Humidity sensor not available.\n");
    }

    app_data_buffer[app_data_size++] = (uint8_t)((temp >> 8) & 0xFF);
    app_data_buffer[app_data_size++] = (uint8_t)(temp & 0xFF);
    app_data_buffer[app_data_size++] = (uint8_t)((humi >> 8) & 0xFF);
    app_data_buffer[app_data_size++] = (uint8_t)(humi & 0xFF);

    send_frame( app_data_buffer, app_data_size, LORAWAN_CONFIRMED_MSG_ON );
}

static void on_modem_tx_done( smtc_modem_event_txdone_status_t status )
{
    printf("on_modem_tx_done\n");
    static uint32_t uplink_count = 0;

    HAL_DBG_TRACE_INFO( "Uplink count: %d\n", ++uplink_count );
}

static void on_modem_down_data( int8_t rssi, int8_t snr, smtc_modem_event_downdata_window_t rx_window, uint8_t port,
                                const uint8_t* payload, uint8_t size )
{
    printf("on_modem_down_data\n");
    HAL_DBG_TRACE_INFO( "Downlink received:\n" );
    HAL_DBG_TRACE_INFO( "  - LoRaWAN Fport = %d\n", port );
    HAL_DBG_TRACE_INFO( "  - Payload size  = %d\n", size );
    HAL_DBG_TRACE_INFO( "  - RSSI          = %d dBm\n", rssi - 64 );
    HAL_DBG_TRACE_INFO( "  - SNR           = %d dB\n", snr >> 2 );

    switch( rx_window )
    {
    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RX1:
    {
        HAL_DBG_TRACE_INFO( "  - Rx window     = %s\n", xstr( SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RX1 ) );
        break;
    }
    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RX2:
    {
        HAL_DBG_TRACE_INFO( "  - Rx window     = %s\n", xstr( SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RX2 ) );
        break;
    }
    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXC:
    {
        HAL_DBG_TRACE_INFO( "  - Rx window     = %s\n", xstr( SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXC ) );
        break;
    }
    }

    if( size != 0 )
    {
        HAL_DBG_TRACE_ARRAY( "Payload", payload, size );
    }
}

static void send_frame( const uint8_t* buffer, const uint8_t length, bool tx_confirmed )
{
    printf("send_frame\n");
    uint8_t tx_max_payload;
    int32_t duty_cycle;

    /* Check if duty cycle is available */
    ASSERT_SMTC_MODEM_RC( smtc_modem_get_duty_cycle_status( &duty_cycle ) );
    if( duty_cycle < 0 )
    {
        HAL_DBG_TRACE_WARNING( "Duty-cycle limitation - next possible uplink in %d ms \n\n", duty_cycle );
        return;
    }

    ASSERT_SMTC_MODEM_RC( smtc_modem_get_next_tx_max_payload( stack_id, &tx_max_payload ) );
    if( length > tx_max_payload )
    {
        HAL_DBG_TRACE_WARNING( "Not enough space in buffer - send empty uplink to flush MAC commands \n" );
        ASSERT_SMTC_MODEM_RC( smtc_modem_request_empty_uplink( stack_id, true, LORAWAN_APP_PORT, tx_confirmed ) );
    }
    else
    {
        HAL_DBG_TRACE_INFO( "Request uplink\n" );
        ASSERT_SMTC_MODEM_RC( smtc_modem_request_uplink( stack_id, LORAWAN_APP_PORT, tx_confirmed, buffer, length ) );
    }
}

void apps_modem_common_configure_lorawan_params( uint8_t stack_id )
{
    printf("apps_modem_common_configure_lorawan_params\n");

    smtc_modem_return_code_t rc = SMTC_MODEM_RC_OK;
    uint8_t dev_eui[8] = { 0 };
    uint8_t join_eui[8]  = { 0 };
    uint8_t app_key[16] = { 0 };

    hal_hex_to_bin( LORAWAN_DEVICE_EUI, dev_eui, 8 );
    hal_hex_to_bin( LORAWAN_JOIN_EUI, join_eui, 8 );
    hal_hex_to_bin( LORAWAN_APP_KEY, app_key, 16 );

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

/* --- EOF ------------------------------------------------------------------ */
