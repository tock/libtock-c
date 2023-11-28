/*!
 * @file      lr11xx_driver_extension.h
 *
 * @brief     driver extension for LR11XX
 *
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

#ifndef LR11XX_DRIVER_EXTENSION_H
#define LR11XX_DRIVER_EXTENSION_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include "lr11xx_types.h"
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

/*!
 * @brief Structure for information about visible SV
 */
typedef struct lr11xx_gnss_visible_satellite_s
{
    lr11xx_gnss_satellite_id_t satellite_id;   //!< SV ID
    int16_t                    doppler;        //!< SV doppler in Hz
    int16_t                    doppler_error;  //!< SV doppler error - step of 125Hz
} lr11xx_gnss_visible_satellite_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/*!
 * @brief Function to read the frequency search space around the Doppler frequency
 *
 * @param [in] radio Radio abstraction
 * @param [out] freq_search_space Frequency search space configuration read from the chip
 *
 * @returns Operation status
 */
lr11xx_status_t lr11xx_gnss_read_freq_search_space( const void*                      radio,
                                                    lr11xx_gnss_freq_search_space_t* freq_search_space );

/*!
 * @brief Function to set the frequency search space around the Doppler frequency
 *
 * @param [in] radio Radio abstraction
 * @param [in] freq_search_space Frequency search space configuration to be applied
 *
 * @returns Operation status
 */
lr11xx_status_t lr11xx_gnss_set_freq_search_space( const void*                           radio,
                                                   const lr11xx_gnss_freq_search_space_t freq_search_space );

/**
 * @brief Return the theoretical number of visible satellites based on the given parameters.
 *
 * @param [in] context Chip implementation context
 * @param [in] date The actual date of scan. Its format is the number of seconds elapsed since January the 6th 1980
 * 00:00:00 with leap seconds included.
 * @param [in] assistance_position, latitude 12 bits and longitude 12 bits
 * @param [in] constellation Bit mask of the constellations to use. See @ref lr11xx_gnss_constellation_t for
 * the possible values
 * @param [out] nb_visible_sv thoeretical number of visible satellites
 *
 * @returns Operation status
 */
lr11xx_status_t lr11xx_gnss_get_nb_visible_satellites(
    const void* context, const lr11xx_gnss_date_t date,
    const lr11xx_gnss_solver_assistance_position_t* assistance_position,
    const lr11xx_gnss_constellation_t constellation, uint8_t* nb_visible_sv );

/**
 * @brief Return the doppler information of theoretical visible satellites, this function shall be called after
 * lr11xx_gnss_get_nb_visible_satellites function.
 *
 * @param [in] context Chip implementation context
 * @param [in] nb_visible_satellites number of visible satellites returned by lr11xx_gnss_get_nb_visible_satellites
 * function,
 * @param [out] visible_satellite_id_doppler Doppler information of each satellite.
 *
 * @returns Operation status
 */
lr11xx_status_t lr11xx_gnss_get_visible_satellites( const void* context, const uint8_t nb_visible_satellites,
                                                    lr11xx_gnss_visible_satellite_t* visible_satellite_id_doppler );

#ifdef __cplusplus
}
#endif

#endif  // LR11XX_DRIVER_EXTENSION_H

/* --- EOF ------------------------------------------------------------------ */