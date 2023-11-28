/**
 * @file      wifi_helpers.c
 *
 * @brief     Interface between the WI-Fi middleware and the LR11xx radio driver.
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

#include <string.h>

#include "mw_assert.h"
#include "mw_dbg_trace.h"

#include "lr11xx_wifi.h"
#include "lr11xx_system.h"

#include "wifi_helpers.h"

#include "mw_bsp.h"
#include "mw_common.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

#define TIMESTAMP_AP_PHONE_FILTERING 18000

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

static wifi_settings_t settings = { 0 };

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

void smtc_wifi_settings_init( const wifi_settings_t* wifi_settings )
{
    /* Set current context Wi-Fi settings */
    memcpy( &settings, wifi_settings, sizeof settings );
}

bool smtc_wifi_start_scan( const void* radio_context )
{
    lr11xx_status_t status;

    if( mw_radio_configure_for_scan( radio_context ) == true )
    {
        status =
            lr11xx_system_set_dio_irq_params( radio_context, LR11XX_SYSTEM_IRQ_WIFI_SCAN_DONE, LR11XX_SYSTEM_IRQ_NONE );
        if( status != LR11XX_STATUS_OK )
        {
            MW_DBG_TRACE_ERROR( "Failed to set Wi-Fi IRQ params\n" );
            return false;
        }

        status = lr11xx_wifi_cfg_timestamp_ap_phone( radio_context, TIMESTAMP_AP_PHONE_FILTERING );
        if( status != LR11XX_STATUS_OK )
        {
            MW_DBG_TRACE_ERROR( "Failed to configure timestamp ap phone\n" );
            return false;
        }

        /* Enable Wi-Fi path */
        mw_bsp_wifi_prescan_actions( );

        status = lr11xx_wifi_scan_time_limit( radio_context, settings.types, settings.channels,
                                              LR11XX_WIFI_SCAN_MODE_BEACON_AND_PKT, settings.max_results,
                                              settings.timeout_per_channel, settings.timeout_per_scan );
        if( status != LR11XX_STATUS_OK )
        {
            MW_DBG_TRACE_ERROR( "Failed to start Wi-Fi scan\n" );
            mw_bsp_wifi_postscan_actions( );
            return false;
        }
    }
    else
    {
        MW_DBG_TRACE_ERROR( "Failed to configure LR11XX for Wi-Fi scan\n" );
        return false;
    }

    return true;
}

void smtc_wifi_scan_ended( void )
{
    /* Disable the Wi-Fi path */
    mw_bsp_wifi_postscan_actions( );
}

bool smtc_wifi_get_results( const void* radio_context, wifi_scan_all_result_t* wifi_results )
{
    lr11xx_wifi_basic_complete_result_t wifi_results_mac_addr[WIFI_MAX_RESULTS] = { 0 };
    uint8_t                             nb_results;
    uint8_t                             max_nb_results;
    uint8_t                             result_index = 0;
    lr11xx_status_t                     status       = LR11XX_STATUS_OK;

    status = lr11xx_wifi_get_nb_results( radio_context, &nb_results );
    if( status != LR11XX_STATUS_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to get Wi-Fi scan number of results\n" );
        return false;
    }

    /* check if the array is big enough to hold all results */
    max_nb_results = sizeof( wifi_results_mac_addr ) / sizeof( wifi_results_mac_addr[0] );
    if( nb_results > max_nb_results )
    {
        MW_DBG_TRACE_ERROR( "Wi-Fi scan result size exceeds %u (%u)\n", max_nb_results, nb_results );
        return false;
    }

    status = lr11xx_wifi_read_basic_complete_results( radio_context, 0, nb_results, wifi_results_mac_addr );
    if( status != LR11XX_STATUS_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to read Wi-Fi scan results\n" );
        return false;
    }

    /* add scan to results */
    for( uint8_t index = 0; index < nb_results; index++ )
    {
        const lr11xx_wifi_basic_complete_result_t* local_basic_result = &wifi_results_mac_addr[index];
        lr11xx_wifi_channel_t                      channel;
        bool                                       rssi_validity;
        lr11xx_wifi_mac_origin_t                   mac_origin_estimation;

        lr11xx_wifi_parse_channel_info( local_basic_result->channel_info_byte, &channel, &rssi_validity,
                                        &mac_origin_estimation );

        if( mac_origin_estimation != LR11XX_WIFI_ORIGIN_BEACON_MOBILE_AP )
        {
            wifi_results->results[result_index].channel = channel;

            wifi_results->results[result_index].type =
                lr11xx_wifi_extract_signal_type_from_data_rate_info( local_basic_result->data_rate_info_byte );

            memcpy( wifi_results->results[result_index].mac_address, local_basic_result->mac_address,
                    LR11XX_WIFI_MAC_ADDRESS_LENGTH );

            wifi_results->results[result_index].rssi = local_basic_result->rssi;
            wifi_results->nbr_results++;
            result_index++;
        }
    }

    return true;
}

bool smtc_wifi_get_power_consumption( const void* radio_context, uint32_t* power_consumption_uah )
{
    lr11xx_status_t                  status;
    lr11xx_wifi_cumulative_timings_t timing;
    lr11xx_system_reg_mode_t         reg_mode;

    status = lr11xx_wifi_read_cumulative_timing( radio_context, &timing );
    if( status != LR11XX_STATUS_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to get wifi timings\n" );
        return false;
    }

    mw_bsp_get_lr11xx_reg_mode( radio_context, &reg_mode );
    *power_consumption_uah = ( uint32_t ) lr11xx_wifi_get_consumption( reg_mode, timing );

    /* Accumulate timings until there is a significant amount of energy consumed */
    if( *power_consumption_uah > 0 )
    {
        status = lr11xx_wifi_reset_cumulative_timing( radio_context );
        if( status != LR11XX_STATUS_OK )
        {
            MW_DBG_TRACE_ERROR( "Failed to reset wifi timings\n" );
            return false;
        }
    }

    return true;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/* --- EOF ------------------------------------------------------------------ */
