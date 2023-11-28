/**
 * @file      wifi_middleware.h
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

#ifndef __WIFI_MIDDLEWARE_H__
#define __WIFI_MIDDLEWARE_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include <stdbool.h>

#include "ralf.h"

#include "mw_common.h"

#include "wifi_helpers_defs.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/**
 * @brief Wi-Fi event status sent from the middleware to the application
 */
typedef enum wifi_mw_event_mask_e
{
    WIFI_MW_EVENT_SCAN_CANCELLED = 0,  //!< Scan operation has been cancelled
    WIFI_MW_EVENT_SCAN_DONE      = 1,  //!< Scan operation has been completed
    WIFI_MW_EVENT_TERMINATED     = 2,  //!< Scan & send sequence has been completed
    WIFI_MW_EVENT_ERROR_UNKNOWN  = 3,  //!< Scan operation has failed for an unknown reason
    /* 8 event types max */
} wifi_mw_event_type_t;

/**
 * @brief Wi-Fi payload format (as defined by LR1110 WiFi positioning protocol of LoRaCloud).
 */
typedef enum wifi_mw_payload_format_e
{
    WIFI_MW_PAYLOAD_MAC      = 0x00,  //!< Only the MAC addresses of the detected Access Points are sent
    WIFI_MW_PAYLOAD_MAC_RSSI = 0x01,  //!< Both MAC address and RSSI of detected Access Points are sent
} wifi_mw_payload_format_t;

/**
 * @brief The data that can be retrieved when a WIFI_MW_EVENT_SCAN_DONE event occurs
 */
typedef wifi_scan_all_result_t wifi_mw_event_data_scan_done_t;

/**
 * @brief The data that can be retrieved when a WIFI_MW_EVENT_TERMINATED event occurs.
 */
typedef struct
{
    uint8_t nb_scans_sent;  //!< Number of scans that have been sent over the air
} wifi_mw_event_data_terminated_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */
/**
 * @brief Get version of the Wi-Fi middleware
 *
 * @param [out] version Middleware version
 *
 * @return Middleware return code as defined in @ref mw_return_code_t
 * @retval MW_RC_OK         Command executed without errors
 * @retval MW_RC_FAILED     version is not initialized
 */
mw_return_code_t wifi_mw_get_version( mw_version_t* version );

/**
 * @brief Initialize the Wi-Fi middleware context
 * Must be called only once at startup, before any other call to the middleware.
 *
 * @param [in] modem_radio Interface to access the radio
 * @param [in] stack_id Modem stack ID
 *
 * @return Middleware return code as defined in @ref mw_return_code_t
 * @retval MW_RC_OK         Command executed without errors
 * @retval MW_RC_FAILED     The modem/radio interface is not initialized
 */
mw_return_code_t wifi_mw_init( ralf_t* modem_radio, uint8_t stack_id );

/**
 * @brief Program a Wi-Fi scan & send sequence to start in a given delay
 *
 * @param [in] start_delay Delay in seconds before starting the scan sequence
 *
 * @return Middleware return code as defined in @ref mw_return_code_t
 * @retval MW_RC_OK         Command executed without errors
 * @retval MW_RC_BUSY       A scan sequence is already on-going
 * @retval MW_RC_FAILED     An error occurred while starting the scanning sequence
 */
mw_return_code_t wifi_mw_scan_start( uint32_t start_delay );

/**
 * @brief Cancel the currently programmed Wi-Fi scan & send sequence (if not actually started)
 *
 * @return Middleware return code as defined in @ref mw_return_code_t
 * @retval MW_RC_OK         Command executed without errors
 * @retval MW_RC_BUSY       The scan sequence has already started (cannot be cancelled)
 */
mw_return_code_t wifi_mw_scan_cancel( void );

/**
 * @brief Check if there is a particular event in the "pending events" bitfield
 *
 * @param [in] pending_events Pending events bitfield given when an event occurs
 * @param [in] event Event to search in the pending events bitfield
 *
 * @return a boolean to indicate if the given event is set in the pending events bitfield
 */
bool wifi_mw_has_event( uint8_t pending_events, wifi_mw_event_type_t event );

/**
 * @brief Retrieve the data associated with the WIFI_MW_EVENT_SCAN_DONE event
 *
 * @param [out] data Description of the scan group results
 *
 * @return Middleware return code as defined in @ref mw_return_code_t
 * @retval MW_RC_OK         Command executed without errors
 * @retval MW_RC_FAILED     If the given pointer is NULL or if there is no SCAN_DONE event pending
 */
mw_return_code_t wifi_mw_get_event_data_scan_done( wifi_mw_event_data_scan_done_t* data );

/**
 * @brief Retrieve the data associated with the WIFI_MW_EVENT_TERMINATED event
 *
 * @param [out] data Status of the end of the scan & send sequence
 *
 * @return Middleware return code as defined in @ref mw_return_code_t
 * @retval MW_RC_OK         Command executed without errors
 * @retval MW_RC_FAILED     If the given pointer is NULL or if there is no TERMINATED event pending
 */
mw_return_code_t wifi_mw_get_event_data_terminated( wifi_mw_event_data_terminated_t* data );

/**
 * @brief Indicates to the middleware that all pending events have been handled and can be cleared
 *
 * Needs to be called when pending events have been processed, to avoid multiple notification of the same event.
 */
void wifi_mw_clear_pending_events( void );

/**
 * @brief Set the LoRaWAN port on which to send the scan results uplinks
 *
 * @param [in] port LoRaWAN port
 *
 * By default it is set to 197 (WIFI_DEFAULT_UPLINK_PORT)
 */
void wifi_mw_set_port( uint8_t port );

/**
 * @brief Bypass the "send" part of the "scan & send" sequence. Basically it is a "scan only" mode.
 * It can be used if the application wants to control how the scan results are sent over the air.
 *
 * @param [in] no_send Boolean to bypass send or not
 *
 * By default it is set to false
 */
void wifi_mw_send_bypass( bool no_send );

/**
 * @brief Indicates the format of the payload to be sent: MAC address only or MAC address with RSSI
 *
 * @param [in] format Payload format to be used
 *
 * By default it is set to MAC address only
 */
void wifi_mw_set_payload_format( wifi_mw_payload_format_t format );

/**
 * @brief Print the data received with the WIFI_MW_EVENT_SCAN_DONE event
 *
 * @param [in] data Scan results to be printed on the console
 */
void wifi_mw_display_results( const wifi_mw_event_data_scan_done_t* data );

#ifdef __cplusplus
}
#endif

#endif  // __WIFI_MIDDLEWARE_H__

/* --- EOF ------------------------------------------------------------------ */