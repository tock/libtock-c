/*!
 * @file      lr11xx_driver_extension.c
 *
 * @brief     driver extension implementation for LR11XX
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

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include "lr11xx_hal.h"
#include "lr11xx_driver_extension.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

#define LR11XX_GNSS_SET_FREQ_SEARCH_SPACE_CMD_LENGTH ( 2 + 1 )
#define LR11XX_GNSS_READ_FREQ_SEARCH_SPACE_CMD_LENGTH ( 2 )
#define LR11XX_GNSS_GET_SV_VISIBLE_CMD_LENGTH ( 2 + 9 )
#define LR11XX_GNSS_GET_SV_VISIBLE_DOPPLER_CMD_LENGTH ( 2 )

#define LR11XX_GNSS_SCALING_LATITUDE 90
#define LR11XX_GNSS_SCALING_LONGITUDE 180

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*!
 * @brief Operating codes for GNSS-related operations
 */
enum
{
    LR11XX_GNSS_SET_FREQ_SEARCH_SPACE_OC  = 0x0404,  //!< Set the frequency search space
    LR11XX_GNSS_READ_FREQ_SEARCH_SPACE_OC = 0x0405,  //!< Read the frequency search space
    LR11XX_GNSS_GET_SV_VISIBLE_OC         = 0x041F,  //!< Get the number of visible SV from a date and a position
    LR11XX_GNSS_GET_SV_VISIBLE_DOPPLER_OC = 0x0420,  //!< Get visible SV ID and corresponding doppler value
};

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

lr11xx_status_t lr11xx_gnss_set_freq_search_space( const void*                           radio,
                                                   const lr11xx_gnss_freq_search_space_t freq_search_space )
{
    const uint8_t cbuffer[LR11XX_GNSS_SET_FREQ_SEARCH_SPACE_CMD_LENGTH] = {
        ( uint8_t )( LR11XX_GNSS_SET_FREQ_SEARCH_SPACE_OC >> 8 ),
        ( uint8_t )( LR11XX_GNSS_SET_FREQ_SEARCH_SPACE_OC >> 0 ),
        ( uint8_t ) freq_search_space,
    };

    return ( lr11xx_status_t ) lr11xx_hal_write( radio, cbuffer, LR11XX_GNSS_SET_FREQ_SEARCH_SPACE_CMD_LENGTH, 0, 0 );
}

lr11xx_status_t lr11xx_gnss_read_freq_search_space( const void*                      radio,
                                                    lr11xx_gnss_freq_search_space_t* freq_search_space )
{
    const uint8_t cbuffer[LR11XX_GNSS_READ_FREQ_SEARCH_SPACE_CMD_LENGTH] = {
        ( uint8_t )( LR11XX_GNSS_READ_FREQ_SEARCH_SPACE_OC >> 8 ),
        ( uint8_t )( LR11XX_GNSS_READ_FREQ_SEARCH_SPACE_OC >> 0 ),
    };

    return ( lr11xx_status_t ) lr11xx_hal_read( radio, cbuffer, LR11XX_GNSS_READ_FREQ_SEARCH_SPACE_CMD_LENGTH,
                                                ( uint8_t* ) freq_search_space, sizeof( uint8_t ) );
}

lr11xx_status_t lr11xx_gnss_get_nb_visible_satellites(
    const void* context, const lr11xx_gnss_date_t date,
    const lr11xx_gnss_solver_assistance_position_t* assistance_position,
    const lr11xx_gnss_constellation_t constellation, uint8_t* nb_visible_sv )
{
    const int16_t latitude  = ( ( assistance_position->latitude * 2048 ) / LR11XX_GNSS_SCALING_LATITUDE );
    const int16_t longitude = ( ( assistance_position->longitude * 2048 ) / LR11XX_GNSS_SCALING_LONGITUDE );
    const uint8_t cbuffer[LR11XX_GNSS_GET_SV_VISIBLE_CMD_LENGTH] = {
        ( uint8_t )( LR11XX_GNSS_GET_SV_VISIBLE_OC >> 8 ),
        ( uint8_t )( LR11XX_GNSS_GET_SV_VISIBLE_OC >> 0 ),
        ( uint8_t )( date >> 24 ),
        ( uint8_t )( date >> 16 ),
        ( uint8_t )( date >> 8 ),
        ( uint8_t )( date >> 0 ),
        ( uint8_t )( latitude >> 8 ),
        ( uint8_t )( latitude >> 0 ),
        ( uint8_t )( longitude >> 8 ),
        ( uint8_t )( longitude >> 0 ),
        ( uint8_t )( constellation - 1 ),
    };

    return ( lr11xx_status_t ) lr11xx_hal_read( context, cbuffer, LR11XX_GNSS_GET_SV_VISIBLE_CMD_LENGTH, nb_visible_sv,
                                                1 );
}

lr11xx_status_t lr11xx_gnss_get_visible_satellites( const void* context, const uint8_t nb_visible_satellites,
                                                    lr11xx_gnss_visible_satellite_t* visible_satellite_id_doppler )
{
    uint8_t        result_buffer[12 * 5] = { 0 };
    const uint16_t read_size             = nb_visible_satellites * 5;

    const uint8_t cbuffer[LR11XX_GNSS_GET_SV_VISIBLE_DOPPLER_CMD_LENGTH] = {
        ( uint8_t )( LR11XX_GNSS_GET_SV_VISIBLE_DOPPLER_OC >> 8 ),
        ( uint8_t )( LR11XX_GNSS_GET_SV_VISIBLE_DOPPLER_OC >> 0 ),
    };

    const lr11xx_hal_status_t hal_status =
        lr11xx_hal_read( context, cbuffer, LR11XX_GNSS_GET_SV_VISIBLE_DOPPLER_CMD_LENGTH, result_buffer, read_size );

    if( hal_status == LR11XX_HAL_STATUS_OK )
    {
        for( uint8_t index_satellite = 0; index_satellite < nb_visible_satellites; index_satellite++ )
        {
            const uint16_t                   local_result_buffer_index = index_satellite * 5;
            lr11xx_gnss_visible_satellite_t* local_satellite_result    = &visible_satellite_id_doppler[index_satellite];

            local_satellite_result->satellite_id = result_buffer[local_result_buffer_index];
            local_satellite_result->doppler      = ( int16_t )( ( result_buffer[local_result_buffer_index + 1] << 8 ) +
                                                           ( result_buffer[local_result_buffer_index + 2] << 0 ) );
            local_satellite_result->doppler_error =
                ( int16_t )( ( result_buffer[local_result_buffer_index + 3] << 8 ) +
                             ( result_buffer[local_result_buffer_index + 4] << 0 ) );
        }
    }

    return ( lr11xx_status_t ) hal_status;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/* --- EOF ------------------------------------------------------------------ */