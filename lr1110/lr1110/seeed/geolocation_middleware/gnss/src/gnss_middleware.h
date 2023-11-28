/**
 * @file      gnss_middleware.h
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

#ifndef __GNSS_MIDDLEWARE_H__
#define __GNSS_MIDDLEWARE_H__

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

#include "gnss_helpers_defs.h"
#include "gnss_queue_defs.h"

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
 * @brief GNSS scanning modes. Configures the scanning sequence according to the use case.
 */
typedef enum
{
    GNSS_MW_MODE_STATIC,  //!< Scanning mode for non moving objects
    GNSS_MW_MODE_MOBILE,  //!< Scanning mode for moving objects
    __GNSS_MW_MODE__SIZE  //!< Number of modes available
} gnss_mw_mode_t;

/**
 * @brief GNSS constellations to be scanned (GPS, BEIDOU or both)
 */
typedef enum
{
    GNSS_MW_CONSTELLATION_GPS,        //!< Use GPS only constellation
    GNSS_MW_CONSTELLATION_BEIDOU,     //!< Use BEIDOU only constellation
    GNSS_MW_CONSTELLATION_GPS_BEIDOU  //!< Use both GPS and BEIDOU constellations
} gnss_mw_constellation_t;

/**
 * @brief GNSS event status sent from the middleware to the application.
 */
typedef enum
{
    GNSS_MW_EVENT_SCAN_CANCELLED       = 0,  //!< Scan operation has been cancelled
    GNSS_MW_EVENT_SCAN_DONE            = 1,  //!< Scan operation has been completed
    GNSS_MW_EVENT_TERMINATED           = 2,  //!< Scan & send sequence has been completed
    GNSS_MW_EVENT_ERROR_NO_TIME        = 3,  //!< Scan operation has failed because no time is available
    GNSS_MW_EVENT_ERROR_ALMANAC_UPDATE = 4,  //!< Scan operation has failed because the almanac needs to be updated
    GNSS_MW_EVENT_ERROR_NO_AIDING_POSITION =
        5,                            //!< Scan operation has failed because the assistance position is not configured
    GNSS_MW_EVENT_ERROR_UNKNOWN = 6,  //!< Scan operation has failed for an unknown reason
    /* 8 event types max */
} gnss_mw_event_type_t;

/**
 * @brief The configuration context in which a scan has been performed.
 */
typedef struct
{
    gnss_mw_mode_t mode;                       //!< Scan mode that has been used (STATIC, MOBILE...)
    bool           assisted;                   //!< Was it an an autonomous scan or an assisted scan ?
    float          aiding_position_latitude;   //!< Aiding position latitude used for the assisted scan
    float          aiding_position_longitude;  //!< Aiding position longitude used for the assisted scan
    uint32_t       almanac_crc;                //!< Almanac CRC when scan was performed
    uint32_t       gps_time;                   //!< GPS time when the scan was launched
    bool almanac_update_checked;   //!< Has the almanac update status been already checked for this scan group ?
    bool almanac_update_required;  //!< Does the almanac requires to be updated ?
} gnss_mw_scan_context_t;

/**
 * @brief Information about detected satellite (Space Vehicle).
 */
typedef struct
{
    uint8_t sv_id;  //!< ID of the space vehicle detected while scanning
    int8_t  cnr;    //!< Carrier to Noise Ratio at which the Space Vehicle has been detected
} gnss_mw_sv_info_t;

/**
 * @brief Description of a scan result.
 */
typedef struct
{
    uint32_t          timestamp;  //!< Scan timestamp
    uint8_t*          nav;        //!< NAV message result for this scan
    uint8_t           nav_size;   //!< NAV message size
    bool              nav_valid;  //!< is the NAV message valid (can be used by the solver for a single frame solve)
    uint8_t           nb_svs;     //!< Number of Space Vehicles detected by this scan
    gnss_mw_sv_info_t info_svs[GNSS_NB_SVS_MAX];  //!< Information about the SVs detected
} gnss_mw_event_data_scan_desc_t;

/**
 * @brief The data that can be retrieved when a GNSS_MW_EVENT_SCAN_DONE event occurs.
 */
typedef struct
{
    bool                           is_valid;        //!< Is the scan group valid ? (enough SV detected...)
    uint8_t                        token;           //!< Scan group identifier
    uint8_t                        nb_scans_valid;  //!< Number of valid scans in that scan group
    gnss_mw_event_data_scan_desc_t scans[GNSS_SCAN_GROUP_SIZE_MAX];  //!< Descriptions of all scan results
    uint32_t                       power_consumption_uah;            //!< Power consumption induced by this scan group
    gnss_mw_scan_context_t         context;                          //!< Configuration context used for this scan
} gnss_mw_event_data_scan_done_t;

/**
 * @brief The data that can be retrieved when a GNSS_MW_EVENT_TERMINATED event occurs.
 */
typedef struct
{
    uint8_t nb_scans_sent;               //!< Number of scans which have been sent over the air
    bool    aiding_position_check_sent;  //!< Indicates if an aiding position check uplink has been sent
    bool    indoor_detected;  //!< Indicates if an indoor detection occurred (in case aiding position check was enabled)
} gnss_mw_event_data_terminated_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/**
 * @brief Get version of the GNSS middleware
 *
 * @param [out] version Middleware version
 *
 * @return Middleware return code as defined in @ref mw_return_code_t
 * @retval MW_RC_OK         Command executed without errors
 * @retval MW_RC_FAILED     version is not initialized
 */
mw_return_code_t gnss_mw_get_version( mw_version_t* version );

/**
 * @brief Initialize the GNSS middleware
 *
 * @param [in] modem_radio Interface to access the radio
 * @param [in] stack_id Modem stack ID
 *
 * @return Middleware return code as defined in @ref mw_return_code_t
 * @retval MW_RC_OK         Command executed without errors
 * @retval MW_RC_FAILED     The modem/radio interface is not initialized
 */
mw_return_code_t gnss_mw_init( ralf_t* modem_radio, uint8_t stack_id );

/**
 * @brief Set the aiding position (assistance position) to be used for assisted scan
 *
 * @param [in] latitude Latitude of the aiding position
 * @param [in] longitude Longitude of the aiding position
 *
 * @return Middleware return code as defined in @ref mw_return_code_t
 * @retval MW_RC_OK         Command executed without errors
 * @retval MW_RC_FAILED     The modem/radio interface is not initialized
 */
mw_return_code_t gnss_mw_set_user_aiding_position( float latitude, float longitude );

/**
 * @brief Program a GNSS scan & send sequence to start in a given delay
 *
 * @param [in] mode Scanning mode to be used (STATIC, MOBILE...)
 * @param [in] start_delay Delay in seconds before starting the scan sequence
 *
 * @return Middleware return code as defined in @ref mw_return_code_t
 * @retval MW_RC_OK         Command executed without errors
 * @retval MW_RC_BUSY       A scan sequence is already on-going
 * @retval MW_RC_FAILED     An error occurred while starting the scanning sequence
 */
mw_return_code_t gnss_mw_scan_start( gnss_mw_mode_t mode, uint32_t start_delay );

/**
 * @brief Cancel the current programmed GNSS scan & send sequence (if not already started)
 * After calling this function, in case of success, the user should wait for the GNSS_MW_EVENT_SCAN_CANCELLED event
 *
 * @return Middleware return code as defined in @ref mw_return_code_t
 * @retval MW_RC_OK         Command executed without errors
 * @retval MW_RC_BUSY       The scan sequence has already started (cannot be cancelled)
 */
mw_return_code_t gnss_mw_scan_cancel( void );

/**
 * @brief Check if there is a particular event in the "pending events" bitfield
 *
 * @param [in] pending_events Pending events bitfield given when an event occurs
 * @param [in] event Event to search in the pending events bitfield
 *
 * @return a boolean to indicate if the given event is set in the pending events bitfield
 */
bool gnss_mw_has_event( uint8_t pending_events, gnss_mw_event_type_t event );

/**
 * @brief Retrieve the data associated with the GNSS_MW_EVENT_SCAN_DONE event
 *
 * @param [out] data Description of the scan group results
 *
 * @return Middleware return code as defined in @ref mw_return_code_t
 * @retval MW_RC_OK         Command executed without errors
 * @retval MW_RC_FAILED     If the given pointer is NULL or if there is no SCAN_DONE event pending
 */
mw_return_code_t gnss_mw_get_event_data_scan_done( gnss_mw_event_data_scan_done_t* data );

/**
 * @brief Retrieve the data associated with the GNSS_MW_EVENT_TERMINATED event
 *
 * @param [out] data Status of the end of the scan & send sequence
 *
 * @return Middleware return code as defined in @ref mw_return_code_t
 * @retval MW_RC_OK         Command executed without errors
 * @retval MW_RC_FAILED     If the given pointer is NULL or if there is no TERMINATED event pending
 */
mw_return_code_t gnss_mw_get_event_data_terminated( gnss_mw_event_data_terminated_t* data );

/**
 * @brief Indicates to the middleware that all pending events have been handled and can be cleared
 */
void gnss_mw_clear_pending_events( void );

/**
 * @brief Set the GNSS constellations to be used for scanning for all subsequent scans (optional)
 *
 * @param [in] constellations Constellation(s) to be used for the scans
 *
 * By default it is configured for using both GPS and BEIDOU constellations
 */
void gnss_mw_set_constellations( gnss_mw_constellation_t constellations );

/**
 * @brief Set the LoRaWAN port on which to send the scan results uplinks
 *
 * @param [in] port LoRaWAN port
 *
 * By default it is set to 192 (GNSS_DEFAULT_UPLINK_PORT)
 */
void gnss_mw_set_port( uint8_t port );

/**
 * @brief Indicates if the current scan group identifier (token) has to be kept unchanged for all subsequent scan & send
 * sequences. It can be used for non-mobile objects to aggregate multiple scan results and use more frames
 * in a multiframe solve to get more accurate position overtime.
 *
 * @param [in] aggregate Boolean to aggregate or not
 *
 * By default it is set to false, meaning that the token will change for each call to gnss_mw_scan_start() if the result
 * was valid
 */
void gnss_mw_scan_aggregate( bool aggregate );

/**
 * @brief Bypass the "send" part of the "scan & send" sequence. Basically it is a "scan only" mode.
 * It can be used if the application wants to control how the scan results are sent over the air.
 *
 * @param [in] no_send Boolean to bypass send or not
 *
 * By default it is set to false
 */
void gnss_mw_send_bypass( bool no_send );

/**
 * @brief Print the results of the GNSS_MW_EVENT_SCAN_DONE event
 *
 * @param [in] data Scan results to be printed on the console
 */
void gnss_mw_display_results( const gnss_mw_event_data_scan_done_t* data );

/**
 * @brief Parse downlink message, and handle it if it targets the GNSS middleware.
 *
 * @param [in] port Port on which the downlink has been received
 * @param [in] payload Payload of the downlink has received
 * @param [in] size Size of the downlink payload
 *
 * @return Middleware return code as defined in @ref mw_return_code_t
 * @retval MW_RC_OK         Command executed without errors
 * @retval MW_RC_FAILED     If the given pointer is NULL or if the size is not compatible with supported messages
 */
mw_return_code_t gnss_mw_handle_downlink( uint8_t port, const uint8_t* payload, uint8_t size );

#ifdef __cplusplus
}
#endif

#endif  // __GNSS_MIDDLEWARE_H__

/* --- EOF ------------------------------------------------------------------ */