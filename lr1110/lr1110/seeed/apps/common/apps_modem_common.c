/*!
 * @file      apps_modem_common.c
 *
 * @brief     Common functions shared by the examples
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

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "apps_modem_common.h"
#include "apps_utilities.h"
#include "lorawan_key_config.h"
#include "smtc_modem_api.h"
#include "smtc_basic_modem_lr11xx_api_extension.h"
#include "smtc_board.h"
#include "smtc_hal.h"
#include "smtc_modem_api_str.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/*!
 * @brief Offset in second between GPS EPOCH and UNIX EPOCH time
 */
#define OFFSET_BETWEEN_GPS_EPOCH_AND_UNIX_EPOCH 315964800

/*!
 * @brief Number of leap seconds as of September 15th 2021
 */
#define OFFSET_LEAP_SECONDS 18

/*!
 * @brief Size of the buffer used as placeholder for the human-readable time
 */
#define TIME_BUFFER_SIZE 80

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
 * --- PUBLIC VARIABLES --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

uint32_t apps_modem_common_get_gps_time( void )
{
    uint32_t gps_time_s       = 0;
    uint32_t gps_fractional_s = 0;

    const smtc_modem_return_code_t status = smtc_modem_get_time( &gps_time_s, &gps_fractional_s );

    switch( status )
    {
    case SMTC_MODEM_RC_OK:
    {
        HAL_DBG_TRACE_INFO( "Current UTC time: %d s\n", gps_time_s );

        break;
    }
    case SMTC_MODEM_RC_NO_TIME:
    {
        HAL_DBG_TRACE_WARNING( "No time available.\n" );
        break;
    }
    default:
    {
        HAL_DBG_TRACE_ERROR( "Cannot get time from modem\n" );
        break;
    }
    }

    return gps_time_s;
}

uint32_t apps_modem_common_get_utc_time( void )
{
    uint32_t gps_time_s       = 0;
    uint32_t gps_fractional_s = 0;
    time_t   time_utc         = 0;

    const smtc_modem_return_code_t status = smtc_modem_get_time( &gps_time_s, &gps_fractional_s );

    switch( status )
    {
    case SMTC_MODEM_RC_OK:
    {
        time_utc = ( time_t )( gps_time_s + OFFSET_BETWEEN_GPS_EPOCH_AND_UNIX_EPOCH - OFFSET_LEAP_SECONDS );

        char             buf[TIME_BUFFER_SIZE];
        const struct tm* time = localtime( &time_utc );  // localtime() is used here instead of gmtime() because the
                                                         // latter is not implemented in the libraries used by Keil

        strftime( buf, TIME_BUFFER_SIZE, "%a %Y-%m-%d %H:%M:%S %Z", time );
        HAL_DBG_TRACE_INFO( "Current UTC time: %s\n", buf );

        break;
    }
    case SMTC_MODEM_RC_NO_TIME:
    {
        HAL_DBG_TRACE_WARNING( "No time available.\n" );
        time_utc = 0;
        break;
    }
    default:
    {
        HAL_DBG_TRACE_ERROR( "Cannot get time from modem\n" );
        time_utc = 0;
        break;
    }
    }

    return ( ( uint32_t ) time_utc );
}

uint32_t apps_modem_common_convert_gps_to_utc_time( uint32_t gps_time_s )
{
    return gps_time_s + OFFSET_BETWEEN_GPS_EPOCH_AND_UNIX_EPOCH - OFFSET_LEAP_SECONDS;
}

void apps_modem_common_configure_lorawan_params( uint8_t stack_id )
{
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
        HAL_DBG_TRACE_ERROR( "smtc_modem_set_deveui failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    rc = smtc_modem_set_joineui( stack_id, join_eui );
    if( rc != SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_ERROR( "smtc_modem_set_joineui failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    rc = smtc_modem_set_nwkkey( stack_id, app_key );
    if( rc != SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_ERROR( "smtc_modem_set_nwkkey failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    HAL_DBG_TRACE_INFO( "LoRaWAN parameters:\n" );

    rc = smtc_modem_get_deveui( stack_id, dev_eui );
    if( rc == SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_ARRAY( "DevEUI", dev_eui, SMTC_MODEM_EUI_LENGTH );
    }
    else
    {
        HAL_DBG_TRACE_ERROR( "smtc_modem_get_deveui failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    rc = smtc_modem_get_joineui( stack_id, join_eui );
    if( rc == SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_ARRAY( "JoinEUI", join_eui, SMTC_MODEM_EUI_LENGTH );
    }
    else
    {
        HAL_DBG_TRACE_ERROR( "smtc_modem_get_joineui failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    rc = smtc_modem_set_class( stack_id, LORAWAN_CLASS );
    if( rc != SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_ERROR( "smtc_modem_set_class failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    modem_class_to_string( LORAWAN_CLASS );

    rc = smtc_modem_set_region( stack_id, LORAWAN_REGION );
    if( rc != SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_ERROR( "smtc_modem_set_region failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    modem_region_to_string( LORAWAN_REGION );

    /* adapt the tx power offet depending on the board */
    rc |= smtc_modem_set_tx_power_offset_db( stack_id, smtc_board_get_tx_power_offset( ) );
}

void apps_modem_common_display_lbm_version( void )
{
    smtc_modem_return_code_t     modem_response_code = SMTC_MODEM_RC_OK;
    smtc_modem_lorawan_version_t lorawan_version;
    smtc_modem_version_t         firmware_version;

    modem_response_code = smtc_modem_get_lorawan_version( &lorawan_version );
    if( modem_response_code == SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_INFO( "LoRaWAN version: %.2x.%.2x.%.2x.%.2x\n", lorawan_version.major, lorawan_version.minor,
                            lorawan_version.patch, lorawan_version.revision );
    }

    modem_response_code = smtc_modem_get_modem_version( &firmware_version );
    if( modem_response_code == SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_INFO( "LoRa Basics Modem version: %.2x.%.2x.%.2x\n", firmware_version.major,
                            firmware_version.minor, firmware_version.patch );
    }
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/* --- EOF ------------------------------------------------------------------ */
