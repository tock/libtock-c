/**
 * @file      gnss_helpers.h
 *
 * @brief     Interface between the GNSS middleware and the LR11xx radio driver.
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

#ifndef __GNSS_HELPERS_H__
#define __GNSS_HELPERS_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include "lr11xx_gnss_types.h"

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
 * @brief Return codes for GNSS get results
 */
typedef enum
{
    SMTC_GNSS_GET_RESULTS_NO_ERROR,           //!< No error
    SMTC_GNSS_GET_RESULTS_ERROR_ALMANAC,      //!< Almanac update required
    SMTC_GNSS_GET_RESULTS_ERROR_AIDING_POS,   //!< No Aiding Position configured
    SMTC_GNSS_GET_RESULTS_ERROR_BUFFER_SIZE,  //!< Results cannot be stored in given buffer
    SMTC_GNSS_GET_RESULTS_ERROR_NO_TIME,      //!< No valid time available
    SMTC_GNSS_GET_RESULTS_ERROR_UNKNOWN       //!< Unkown error
} smtc_gnss_get_results_return_code_t;

/**
 * @brief GNSS scan configuration parameters
 */
typedef struct
{
    bool                             assisted;           //!< assisted or autonomous scan
    lr11xx_gnss_constellation_mask_t constellations;     //!< constellations to be used
    uint8_t                          input_parameters;   //!< scan in put parameters
    lr11xx_gnss_freq_search_space_t  freq_search_space;  //!< frequency search space
    uint8_t                          nb_svs_max;         //!< maximum number of Space Vehicles to be detected
} smtc_gnss_scan_params_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/*!
 * @brief Manually sets an assistance position for assisted scan
 *
 * @param [in] radio_context Chip implementation context
 * @param [in] assistance_position Assistance position to use for next assisted GNSS scan
 *
 * @return a boolean: true for success, false otherwise
 */
bool smtc_gnss_set_assistance_position( const void*                                     radio_context,
                                        const lr11xx_gnss_solver_assistance_position_t* assistance_position );

/*!
 * @brief Push a message received from the geolocation solver to the LR11XX
 *
 * @param [in] radio_context Chip implementation context
 * @param [in] payload Message received from the solver to be pushed to the LR11XX
 * @param [in] payload_size Message payload size
 *
 * @return a boolean: true for success, false otherwise
 */
bool smtc_gnss_push_solver_msg( const void* radio_context, const uint8_t* payload, const uint16_t payload_size );

/*!
 * @brief Get current almanac CRC from LR11xx radio
 *
 * @param [in] radio_context Chip implementation context
 * @param [out] almanac_crc Current almanac CRC stored in LR11XX
 *
 * @return a boolean: true for success, false otherwise
 */
bool smtc_gnss_get_almanac_crc( const void* radio_context, uint32_t* almanac_crc );

/*!
 * @brief Start a GNSS scan
 *
 * @param [in] radio_context Chip implementation context
 * @param [in] date Date to use for GNSS scan operation
 * @param [in] params Pointer to the scan parameters to be used

 * @return a boolean: true for success, false otherwise
 */
bool smtc_gnss_scan( const void* radio_context, uint32_t date, const smtc_gnss_scan_params_t* params );

/*!
 * @brief Execute tear down actions when GNSS scan operation is terminated
 */
void smtc_gnss_scan_ended( void );

/*!
 * @brief Fetch the result of last GNSS scan operation
 *
 * @param [in] radio_context Chip implementation context
 * @param [in] results_max_size Size of the results array given to store scan results
 * @param [out] res_sz Length of the raw result buffer returned by the radio
 * @param [out] results Pointer to a buffer that will be filled with raw result buffer
 *
 * @return get results return code as defined in @ref smtc_gnss_get_results_return_code_t
 */
smtc_gnss_get_results_return_code_t smtc_gnss_get_results( const void* radio_context, const uint8_t results_max_size,
                                                           uint8_t* res_sz, uint8_t* results, bool* no_sv_detected );

/*!
 * @brief Get detailed info of detected SVs
 *
 * @param [in] radio_context Chip implementation context
 * @param [in] sv_info_max_size Size of the sv_info array to fill with detected space vehicle information
 * @param [out] nb_detected_sv Number of Space Vehicles detected in last scan
 * @param [out] sv_info Pointer to an array of structures of size big enough to contain
 * nb_detected_sv elements. Information about Space Vehicles detected in last scan
 *
 * @return a boolean: true for success, false otherwise
 */
bool smtc_gnss_get_sv_info( const void* radio_context, const uint8_t sv_info_max_size, uint8_t* nb_detected_sv,
                            lr11xx_gnss_detected_satellite_t* sv_info );

/*!
 * @brief TODO
 */
bool smtc_gnss_get_almanac_update_status( const void* radio_context, const uint32_t date,
                                          const lr11xx_gnss_solver_assistance_position_t* assistance_position,
                                          const lr11xx_gnss_constellation_mask_t          constellations,
                                          bool*                                           almanacs_update_required );

/*!
 * @brief TODO
 */
bool smtc_gnss_get_doppler_error_from_nav( const uint8_t* nav );

/*!
 * @brief TODO
 */
bool smtc_gnss_get_doppler_error( const void* radio_context, const uint32_t date,
                                  const lr11xx_gnss_solver_assistance_position_t* assistance_position,
                                  const lr11xx_gnss_constellation_mask_t constellations, const uint8_t nb_detected_sv,
                                  const lr11xx_gnss_detected_satellite_t* detected_sv_info, bool* doppler_error );

/*!
 * @brief Get the power consumption of the last scan
 *
 * @param [in] radio_context Chip implementation context
 * @param [out] power_consumption_uah Power consumption of the last scan in uAh
 *
 * @return a boolean: true for success, false otherwise
 */
bool smtc_gnss_get_power_consumption( const void* radio_context, uint32_t* power_consumption_uah );

/*!
 * @brief Read the current scan context (almanac CRC, aiding position....)
 *
 * @param [in] radio_context Chip implementation context
 * @param [out] aiding_position Aiding position which is currently written in the LR11xx radio
 * @param [out] almanac_crc Almanac CRC of the current almanac written in the LR11xx radio
 *
 * @return a boolean: true for success, false otherwise.
 */
bool smtc_gnss_get_scan_context( const void* radio_context, lr11xx_gnss_solver_assistance_position_t* aiding_position,
                                 uint32_t* almanac_crc );

/*!
 * @brief Check if the given NAV message is valid (would allow the solver to return a position)
 *
 * @param [in] constellations Constellations used
 * @param [in] nb_detected_satellites Number of detected space vehicles
 * @param [in] detected_satellites Space vehicles detected
 *
 * @return a boolean: true if the NAV message is valid, false otherwise
 */
bool smtc_gnss_is_nav_message_valid( const lr11xx_gnss_constellation_mask_t constellations,
                                     uint8_t                                nb_detected_satellites,
                                     lr11xx_gnss_detected_satellite_t*      detected_satellites );

#ifdef __cplusplus
}
#endif

#endif  // __GNSS_HELPERS_H__

/* --- EOF ------------------------------------------------------------------ */