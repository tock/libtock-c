/**
 * @file      gnss_helpers.c
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

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */
#include <string.h>

#include "gnss_helpers.h"
#include "gnss_helpers_defs.h"

#include "mw_assert.h"
#include "mw_dbg_trace.h"
#include "mw_bsp.h"
#include "mw_common.h"

#include "smtc_modem_api.h"

#include "lr11xx_system.h"
#include "lr11xx_gnss.h"

#include "lr11xx_driver_extension.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

#define GNSS_HELPERS_FEATURE_OFF 0
#define GNSS_HELPERS_FEATURE_ON !GNSS_HELPERS_FEATURE_OFF

#ifndef GNSS_HELPERS_DBG_TRACE
#define GNSS_HELPERS_DBG_TRACE GNSS_HELPERS_FEATURE_OFF /* Enable/Disable traces here */
#endif

#if( GNSS_HELPERS_DBG_TRACE == GNSS_HELPERS_FEATURE_ON )
#define GNSS_HELPERS_TRACE_MSG( msg ) \
    do                                \
    {                                 \
        MW_DBG_TRACE_PRINTF( msg );   \
    } while( 0 )

#define GNSS_HELPERS_TRACE_PRINTF( ... )    \
    do                                      \
    {                                       \
        MW_DBG_TRACE_PRINTF( __VA_ARGS__ ); \
    } while( 0 )

#else
#define GNSS_HELPERS_TRACE_MSG( msg )
#define GNSS_HELPERS_TRACE_PRINTF( ... )
#endif

#define ABS( N ) ( ( N < 0 ) ? ( -N ) : ( N ) )

#define GETBIT( var, bit ) ( ( ( var ) >> ( bit ) ) & 1 )

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

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

/*!
 * @brief Configure and start scan operation (autonomous or assisted)
 *
 * @param[in] radio_context Chip implementation context
 * @param[in] date Current date
 * @param[in] params Pointer to scan parameters

 * @return a boolean: true for success, false otherwise
 */
static bool gnss_scan( const void* radio_context, lr11xx_gnss_date_t date, const smtc_gnss_scan_params_t* params );

/*!
 * @brief Parse a result buffer to determine if it is a NAV message to be sent to the solver
 *
 * @param[in] buffer Buffer containing the result to be parsed
 * @param[in] buffer_length Length of the given result buffer
 *
 * @return a boolean: true for success, false otherwise
 */
static inline bool gnss_is_result_to_solver( const uint8_t* buffer, uint8_t buffer_length );

/*!
 * @brief Parse a result buffer to determine if it is a message for the host (error message)
 *
 * @param[in] buffer Buffer containing the result to be parsed
 * @param[in] buffer_length Length of the given result buffer
 *
 * @return a boolean: true for success, false otherwise
 */
static inline bool gnss_is_result_to_host( const uint8_t* buffer, uint8_t buffer_length );

/*!
 * @brief Compute the median value of the given values array
 *
 * @param[in] n Size of the input array
 * @param[in] x Array containing values
 *
 * @return an integer: the median value
 */
static int median( int n, int x[] );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

bool smtc_gnss_set_assistance_position( const void*                                     radio_context,
                                        const lr11xx_gnss_solver_assistance_position_t* assistance_position )
{
    if( lr11xx_gnss_set_assistance_position( radio_context, assistance_position ) != LR11XX_STATUS_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to set assistance position\n" );
        return false;
    }

    return true;
}

bool smtc_gnss_push_solver_msg( const void* radio_context, const uint8_t* payload, const uint16_t payload_size )
{
    if( lr11xx_gnss_push_solver_msg( radio_context, payload, payload_size ) != LR11XX_STATUS_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to push solver msg\n" );
        return false;
    }

    return true;
}

bool smtc_gnss_get_almanac_crc( const void* radio_context, uint32_t* almanac_crc )
{
    lr11xx_status_t                         err;
    lr11xx_gnss_context_status_bytestream_t context_status_bytestream;
    lr11xx_gnss_context_status_t            context_status;

    err = lr11xx_gnss_get_context_status( radio_context, context_status_bytestream );
    if( err != LR11XX_STATUS_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to get gnss context status\n" );
        return false;
    }

    err = lr11xx_gnss_parse_context_status_buffer( context_status_bytestream, &context_status );
    if( err != LR11XX_STATUS_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to parse gnss context status to get almanac status\n" );
        return false;
    }

    *almanac_crc = context_status.global_almanac_crc;

    return true;
}

bool smtc_gnss_scan( const void* radio_context, uint32_t date, const smtc_gnss_scan_params_t* params )
{
    bool status;

    status = mw_radio_configure_for_scan( radio_context );
    if( status == true )
    {
        /* Enable LNA for GNSS with LR11XX Evaluation board with passive antenna */
        mw_bsp_gnss_prescan_actions( );

        /* Start scan */
        status = gnss_scan( radio_context, ( lr11xx_gnss_date_t ) date, params );
        if( status == false )
        {
            MW_DBG_TRACE_ERROR( "gnss_scan() failed\n" );
            mw_bsp_gnss_postscan_actions( );
            return false;
        }
    }
    else
    {
        MW_DBG_TRACE_ERROR( "Failed to configure LR11XX for GNSS scan\n" );
        return false;
    }

    return true;
}

void smtc_gnss_scan_ended( void )
{
    /* Disable LNA which was enabled during GNSS scan */
    mw_bsp_gnss_postscan_actions( );
}

bool smtc_gnss_get_scan_context( const void* radio_context, lr11xx_gnss_solver_assistance_position_t* aiding_position,
                                 uint32_t* almanac_crc )
{
    lr11xx_status_t status;
    bool            err;

    status = lr11xx_gnss_read_assistance_position( radio_context, aiding_position );
    if( status != LR11XX_STATUS_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to read assistance position from LR11xx\n" );
        return false;
    }

    err = smtc_gnss_get_almanac_crc( radio_context, almanac_crc );
    if( err != true )
    {
        MW_DBG_TRACE_ERROR( "Failed to read almanac CRC from LR11xx\n" );
        return false;
    }

    return true;
}

smtc_gnss_get_results_return_code_t smtc_gnss_get_results( const void* radio_context, const uint8_t results_max_size,
                                                           uint8_t* res_sz, uint8_t* results, bool* no_sv_detected )
{
    lr11xx_status_t status;
    uint16_t        result_size;

    /* Initialize output value, in case this function returns with an error */
    *res_sz         = 0;
    *no_sv_detected = false;

    /* Use read result API to fetch the result buffer */
    status = lr11xx_gnss_get_result_size( radio_context, &result_size );
    if( status != LR11XX_STATUS_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to get GNSS scan result size\n" );
        return SMTC_GNSS_GET_RESULTS_ERROR_UNKNOWN;
    }

    if( result_size > results_max_size )
    {
        MW_DBG_TRACE_ERROR( "GNSS scan result size exceeds %d (%u)\n", results_max_size, result_size );
        return SMTC_GNSS_GET_RESULTS_ERROR_BUFFER_SIZE;
    }

    status = lr11xx_gnss_read_results( radio_context, results, result_size );
    if( status != LR11XX_STATUS_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to get result\n" );
        return SMTC_GNSS_GET_RESULTS_ERROR_UNKNOWN;
    }

    /* Check if the message read from read result API is a NAV message or not. If it is not, print the
    appropriate error message */
    if( gnss_is_result_to_solver( results, result_size ) == false )
    {
        /* The result read is for the solver, it is probably an error message. */
        if( gnss_is_result_to_host( results, result_size ) == true )
        {
            const lr11xx_gnss_message_host_status_t status_code_raw = ( lr11xx_gnss_message_host_status_t ) results[1];
            switch( status_code_raw )
            {
            case LR11XX_GNSS_HOST_NO_TIME:
            {
                MW_DBG_TRACE_ERROR( "GNSS error: NO TIME\n" );
                return SMTC_GNSS_GET_RESULTS_ERROR_NO_TIME;
            }
            case LR11XX_GNSS_HOST_NO_SATELLITE_DETECTED:
            {
                MW_DBG_TRACE_INFO( "GNSS error: NO SATELLITE\n" );
                *no_sv_detected = true;
                return SMTC_GNSS_GET_RESULTS_NO_ERROR; /* not an error */
            }
            case LR11XX_GNSS_HOST_ALMANAC_IN_FLASH_TOO_OLD:
            {
                MW_DBG_TRACE_ERROR( "GNSS error: ALMANAC TOO OLD\n" );
                return SMTC_GNSS_GET_RESULTS_ERROR_ALMANAC;
            }
            case LR11XX_GNSS_HOST_NOT_ENOUGH_SV_DETECTED_TO_BUILD_A_NAV_MESSAGE:
            {
                MW_DBG_TRACE_INFO( "GNSS error: NOT ENOUGH SVs TO BUILD A NAV MESSAGE\n" );
                return SMTC_GNSS_GET_RESULTS_NO_ERROR; /* not an error */
            }
            default:
            {
                MW_DBG_TRACE_ERROR( "GNSS error: UNKNOWN ERROR CODE: 0x%02X\n", status_code_raw );
                return SMTC_GNSS_GET_RESULTS_ERROR_UNKNOWN;
            }
            }
        }
        else
        {
            MW_DBG_TRACE_ERROR(
                "GNSS error: NAV message is neither for host nor for solver. Destination byte: 0x%02x\n", results[0] );
            return SMTC_GNSS_GET_RESULTS_ERROR_UNKNOWN;
        }
    }
    else
    {
        /* The result read is for the solver, check if there is an error status. */
        const uint8_t status_code_raw = results[1];
        if( status_code_raw == 0x00 )
        {
            MW_DBG_TRACE_ERROR( "GNSS error: NO ASSISTANCE POSITION\n" );
            MW_DBG_TRACE_ARRAY( "results", results, result_size );
            return SMTC_GNSS_GET_RESULTS_ERROR_AIDING_POS;
        }
    }

    /* Remove the destination byte (first byte) which does not need to be sent over the air */
    memmove( results, results + 1, result_size - 1 );

    /* Set the returned buffer size without the destination byte */
    *res_sz = result_size - 1;

    return SMTC_GNSS_GET_RESULTS_NO_ERROR;
}

bool smtc_gnss_get_sv_info( const void* radio_context, const uint8_t sv_info_max_size, uint8_t* nb_detected_sv,
                            lr11xx_gnss_detected_satellite_t* sv_info )
{
    lr11xx_status_t status;

    /* Initialize output values, in case this function returns with an error */
    *nb_detected_sv = 0;

    /* Fetch the detected SVs */
    status = lr11xx_gnss_get_nb_detected_satellites( radio_context, nb_detected_sv );
    if( status != LR11XX_STATUS_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to get number of satellites detected\n" );
        return false;
    }

    if( *nb_detected_sv > sv_info_max_size )
    {
        MW_DBG_TRACE_ERROR( "Cannot store info of all detected SVs (%u: max:%u)\n", nb_detected_sv, sv_info_max_size );
        return false;
    }

    /* Get details about detected SVs */
    status = lr11xx_gnss_get_detected_satellites( radio_context, *nb_detected_sv, sv_info );
    if( status != LR11XX_STATUS_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to get detected satellites\n" );
        return false;
    }

    return true;
}

bool smtc_gnss_get_almanac_update_status( const void* radio_context, const uint32_t date,
                                          const lr11xx_gnss_solver_assistance_position_t* assistance_position,
                                          const lr11xx_gnss_constellation_mask_t          constellations,
                                          bool*                                           almanacs_update_required )
{
    lr11xx_status_t                 status;
    uint8_t                         nb_visible_gps_satellites, nb_visible_beidou_satellites;
    lr11xx_gnss_visible_satellite_t visible_gps[12];
    lr11xx_gnss_visible_satellite_t visible_beidou[12];
    uint8_t                         nb_almanac_old = 0;

    /* Initialize output status */
    *almanacs_update_required = false;

    /* Get visible satellites for GPS constellation */
    if( constellations & LR11XX_GNSS_GPS_MASK )
    {
        status = lr11xx_gnss_get_nb_visible_satellites( radio_context, ( lr11xx_gnss_date_t ) date, assistance_position,
                                                        LR11XX_GNSS_GPS_MASK, &nb_visible_gps_satellites );
        if( status != LR11XX_STATUS_OK )
        {
            MW_DBG_TRACE_ERROR( "Failed to get number of visible GPS satellites\n" );
            return false;
        }

        status = lr11xx_gnss_get_visible_satellites( radio_context, nb_visible_gps_satellites, visible_gps );
        if( status != LR11XX_STATUS_OK )
        {
            MW_DBG_TRACE_ERROR( "Failed to get visible GPS satellites info\n" );
            return false;
        }

        GNSS_HELPERS_TRACE_PRINTF( "\nVisible GPS satellites: %u\n", nb_visible_gps_satellites );
        GNSS_HELPERS_TRACE_PRINTF( "id  | doppler    | almanac_age  \n" );
        GNSS_HELPERS_TRACE_PRINTF( "----|------------|--------------\n" );
        for( int i = 0; i < nb_visible_gps_satellites; i++ )
        {
            GNSS_HELPERS_TRACE_PRINTF( "%-3u | %-10d | %-10d\n", visible_gps[i].satellite_id, visible_gps[i].doppler,
                                       visible_gps[i].doppler_error );
        }

        /* Get the ratio of visible GPS SVs which have almanacs up-to-date */
        for( int i = 0; i < nb_visible_gps_satellites; i++ )
        {
            if( visible_gps[i].doppler_error > 250 ) /* almanac age more than 6 months old (125Hz = 3months) */
            {
                nb_almanac_old += 1;
            }
        }
    }

    /* Get visible satellites for BEIDOU constellation */
    if( constellations & LR11XX_GNSS_BEIDOU_MASK )
    {
        status = lr11xx_gnss_get_nb_visible_satellites( radio_context, ( lr11xx_gnss_date_t ) date, assistance_position,
                                                        LR11XX_GNSS_BEIDOU_MASK, &nb_visible_beidou_satellites );
        if( status != LR11XX_STATUS_OK )
        {
            MW_DBG_TRACE_ERROR( "Failed to get number of visible BEIDOU satellites\n" );
            return false;
        }

        status = lr11xx_gnss_get_visible_satellites( radio_context, nb_visible_beidou_satellites, visible_beidou );
        if( status != LR11XX_STATUS_OK )
        {
            MW_DBG_TRACE_ERROR( "Failed to get visible BEIDOU satellites info\n" );
            return false;
        }

        GNSS_HELPERS_TRACE_PRINTF( "\nVisible BEIDOU satellites: %u\n", nb_visible_beidou_satellites );
        GNSS_HELPERS_TRACE_PRINTF( "id  | doppler    | almanac_age  \n" );
        GNSS_HELPERS_TRACE_PRINTF( "----|------------|--------------\n" );
        for( int i = 0; i < nb_visible_beidou_satellites; i++ )
        {
            GNSS_HELPERS_TRACE_PRINTF( "%-3u | %-10d | %-10d\n", visible_beidou[i].satellite_id,
                                       visible_beidou[i].doppler, visible_beidou[i].doppler_error );
        }

        /* Get the ratio of visible BEIDOU SVs which have almanacs up-to-date */
        for( int i = 0; i < nb_visible_beidou_satellites; i++ )
        {
            if( visible_beidou[i].doppler_error > 250 ) /* almanac age more than 8 months old (125Hz = 4months) */
            {
                nb_almanac_old += 1;
            }
        }
    }

    /* Percentage of almanac updated among visible SVs */
    uint16_t almanacs_updated_status =
        100 - ( nb_almanac_old * 100 / ( nb_visible_gps_satellites + nb_visible_beidou_satellites ) );

    /* at least 70% of visible SVs with up-to-date almanacs are necessary to trust doppler error detection */
    if( almanacs_updated_status < 70 )
    {
        *almanacs_update_required = true;
    }
    else
    {
        *almanacs_update_required = false;
    }

    GNSS_HELPERS_TRACE_PRINTF( "Almanac update status: %u%% (update_required:%d)\n", almanacs_updated_status,
                               *almanacs_update_required );

    return true;
}

bool smtc_gnss_get_doppler_error_from_nav( const uint8_t* nav )
{
    uint8_t bit = GETBIT( nav[0], 4 );
    return ( bool ) bit;
}

bool smtc_gnss_get_doppler_error( const void* radio_context, const uint32_t date,
                                  const lr11xx_gnss_solver_assistance_position_t* assistance_position,
                                  const lr11xx_gnss_constellation_mask_t constellations, const uint8_t nb_detected_sv,
                                  const lr11xx_gnss_detected_satellite_t* detected_sv_info, bool* doppler_error )
{
    lr11xx_status_t                 status;
    uint8_t                         nb_visible_gps_satellites, nb_visible_beidou_satellites;
    lr11xx_gnss_visible_satellite_t visible_gps[12];
    lr11xx_gnss_visible_satellite_t visible_beidou[12];

    typedef struct doppler_offset_s
    {
        lr11xx_gnss_satellite_id_t id;
        int16_t                    doppler_offset;
        int16_t                    almanac_age;
    } doppler_offset_t;
    doppler_offset_t doppler_offsets[GNSS_NB_SVS_MAX] = { 0 }; /* doppler offsets of detected SVs */

    int doppler_offsets_for_median_size             = 0;
    int doppler_offsets_for_median[GNSS_NB_SVS_MAX] = { 0 };

    /* Initialize output status */
    *doppler_error = false;

    GNSS_HELPERS_TRACE_PRINTF( "date:%u (%.6f, %.6f)\n", date, assistance_position->latitude,
                               assistance_position->longitude );

    /* Get visible satellites for GPS constellation */
    if( constellations & LR11XX_GNSS_GPS_MASK )
    {
        status = lr11xx_gnss_get_nb_visible_satellites( radio_context, ( lr11xx_gnss_date_t ) date, assistance_position,
                                                        LR11XX_GNSS_GPS_MASK, &nb_visible_gps_satellites );
        if( status != LR11XX_STATUS_OK )
        {
            MW_DBG_TRACE_ERROR( "Failed to get number of visible GPS satellites\n" );
            return false;
        }

        status = lr11xx_gnss_get_visible_satellites( radio_context, nb_visible_gps_satellites, visible_gps );
        if( status != LR11XX_STATUS_OK )
        {
            MW_DBG_TRACE_ERROR( "Failed to get visible GPS satellites info\n" );
            return false;
        }
    }

    /* Get visible satellites for BEIDOU constellation */
    if( constellations & LR11XX_GNSS_BEIDOU_MASK )
    {
        status = lr11xx_gnss_get_nb_visible_satellites( radio_context, ( lr11xx_gnss_date_t ) date, assistance_position,
                                                        LR11XX_GNSS_BEIDOU_MASK, &nb_visible_beidou_satellites );
        if( status != LR11XX_STATUS_OK )
        {
            MW_DBG_TRACE_ERROR( "Failed to get number of visible BEIDOU satellites\n" );
            return false;
        }

        status = lr11xx_gnss_get_visible_satellites( radio_context, nb_visible_beidou_satellites, visible_beidou );
        if( status != LR11XX_STATUS_OK )
        {
            MW_DBG_TRACE_ERROR( "Failed to get visible BEIDOU satellites info\n" );
            return false;
        }
    }

    /* Compute the offset between the detected doppler and the theoretical doppler of visible SVs */
    for( int j = 0; j < nb_detected_sv; j++ )
    {
        /* GPS satellites */
        if( constellations & LR11XX_GNSS_GPS_MASK )
        {
            for( int k = 0; k < nb_visible_gps_satellites; k++ )
            {
                if( detected_sv_info[j].satellite_id == visible_gps[k].satellite_id )
                {
                    doppler_offsets[j].id             = detected_sv_info[j].satellite_id;
                    doppler_offsets[j].doppler_offset = detected_sv_info[j].doppler - visible_gps[k].doppler;
                    doppler_offsets[j].almanac_age    = visible_gps[k].doppler_error;

                    /* Add doppler_offset to median calculation, if almanac is up-to-date */
                    if( doppler_offsets[j].almanac_age == 0 )
                    {
                        doppler_offsets_for_median[doppler_offsets_for_median_size] =
                            ( int ) doppler_offsets[j].doppler_offset;
                        doppler_offsets_for_median_size += 1;
                    }
                }
            }
        }
        /* BEIDOU satellites */
        if( constellations & LR11XX_GNSS_BEIDOU_MASK )
        {
            for( int k = 0; k < nb_visible_beidou_satellites; k++ )
            {
                if( detected_sv_info[j].satellite_id == visible_beidou[k].satellite_id )
                {
                    doppler_offsets[j].id             = detected_sv_info[j].satellite_id;
                    doppler_offsets[j].doppler_offset = detected_sv_info[j].doppler - visible_beidou[k].doppler;
                    doppler_offsets[j].almanac_age    = visible_beidou[k].doppler_error;

                    /* Add doppler_offset to median calculation, if almanac is up-to-date */
                    if( doppler_offsets[j].almanac_age == 0 )
                    {
                        doppler_offsets_for_median[doppler_offsets_for_median_size] =
                            ( int ) doppler_offsets[j].doppler_offset;
                        doppler_offsets_for_median_size += 1;
                    }
                }
            }
        }
    }

    /* Compute the median doppler offset of SVs with updated almanacs */
    int median_offset = median( doppler_offsets_for_median_size, doppler_offsets_for_median );

    /* Debug prints: TODO: add flag to disable/enable */
    GNSS_HELPERS_TRACE_PRINTF( "\nDetected satellites: %u\n", nb_detected_sv );
    GNSS_HELPERS_TRACE_PRINTF( "id  | doppler    | cnr\n" );
    GNSS_HELPERS_TRACE_PRINTF( "----|------------|--------------\n" );
    for( int i = 0; i < nb_detected_sv; i++ )
    {
        GNSS_HELPERS_TRACE_PRINTF( "%-3u | %-10d | %-10d\n", detected_sv_info[i].satellite_id,
                                   detected_sv_info[i].doppler, detected_sv_info[i].cnr );
    }

    if( constellations & LR11XX_GNSS_GPS_MASK )
    {
        GNSS_HELPERS_TRACE_PRINTF( "\nVisible GPS satellites: %u\n", nb_visible_gps_satellites );
        GNSS_HELPERS_TRACE_PRINTF( "id  | doppler    | almanac_age  \n" );
        GNSS_HELPERS_TRACE_PRINTF( "----|------------|--------------\n" );
        for( int i = 0; i < nb_visible_gps_satellites; i++ )
        {
            GNSS_HELPERS_TRACE_PRINTF( "%-3u | %-10d | %-10d\n", visible_gps[i].satellite_id, visible_gps[i].doppler,
                                       visible_gps[i].doppler_error );
        }
    }

    if( constellations & LR11XX_GNSS_BEIDOU_MASK )
    {
        GNSS_HELPERS_TRACE_PRINTF( "\nVisible BEIDOU satellites: %u\n", nb_visible_beidou_satellites );
        GNSS_HELPERS_TRACE_PRINTF( "id  | doppler    | almanac_age  \n" );
        GNSS_HELPERS_TRACE_PRINTF( "----|------------|--------------\n" );
        for( int i = 0; i < nb_visible_beidou_satellites; i++ )
        {
            GNSS_HELPERS_TRACE_PRINTF( "%-3u | %-10d | %-10d\n", visible_beidou[i].satellite_id,
                                       visible_beidou[i].doppler, visible_beidou[i].doppler_error );
        }
    }

    /* Get the current frequency search space configuration to determine threshold for acceptable doppler offset */
    lr11xx_gnss_freq_search_space_t freq_search_space;
    int16_t                         offset_threshold;
    status = lr11xx_gnss_read_freq_search_space( radio_context, &freq_search_space );
    if( status != LR11XX_STATUS_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to read current freq search space\n" );
        return false;
    }
    switch( freq_search_space )
    {
    case LR11XX_GNSS_FREQUENCY_SEARCH_SPACE_250_HZ:
        offset_threshold = 250;
        break;
    case LR11XX_GNSS_FREQUENCY_SEARCH_SPACE_500_HZ:
        offset_threshold = 500;
        break;
    case LR11XX_GNSS_FREQUENCY_SEARCH_SPACE_1_KHZ:
        offset_threshold = 1000;
        break;
    case LR11XX_GNSS_FREQUENCY_SEARCH_SPACE_2_KHZ:
        offset_threshold = 2000;
        break;
    default:
        MW_DBG_TRACE_ERROR( "Unknown freq search space - %d\n", freq_search_space );
        return false;
    }

    /* Check if there is at least 1 SV with doppler error (with almanac up-to-date) */
    int16_t offset_from_median;
    bool    doppler_error_sv;
    bool    doppler_error_ignored;
    GNSS_HELPERS_TRACE_PRINTF( "\nDoppler offsets (median=%d):\n", median_offset );
    GNSS_HELPERS_TRACE_PRINTF( "id  | offset     | off-median | doppler_err\n" );
    GNSS_HELPERS_TRACE_PRINTF( "----|------------|------------|------------\n" );
    for( int i = 0; i < nb_detected_sv; i++ )
    {
        offset_from_median    = ABS( ( int16_t )( doppler_offsets[i].doppler_offset - median_offset ) );
        doppler_error_sv      = ( offset_from_median > offset_threshold ) ? true : false;
        doppler_error_ignored = ( doppler_offsets[i].almanac_age > 250 )
                                    ? true
                                    : false; /* If almanac is too old for this SV, ignore doppler detected */
        if( doppler_error_ignored == true )
        {
            doppler_error_sv = false;
        }
        GNSS_HELPERS_TRACE_PRINTF( "%-3u | %-10d | %-10d | %d %s\n", doppler_offsets[i].id,
                                   doppler_offsets[i].doppler_offset, offset_from_median, doppler_error_sv,
                                   ( doppler_error_ignored == true ) ? "(IGNORED)" : "" );

        if( doppler_error_sv == true )
        {
            /* Return doppler error status */
            *doppler_error = true;
            /* TODO: break here if don't want to have details for all SVs */
        }
    }

    if( *doppler_error == true )
    {
        MW_DBG_TRACE_WARNING( "Doppler error detected\n" );
    }

    return true;
}

bool smtc_gnss_get_power_consumption( const void* radio_context, uint32_t* power_consumption_uah )
{
    lr11xx_status_t                  status;
    lr11xx_gnss_timings_t            timings;
    lr11xx_gnss_constellation_mask_t constellation_used;
    lr11xx_system_reg_mode_t         reg_mode;

    /* Initialize output values, in case this function returns with an error */
    *power_consumption_uah = 0;

    status = lr11xx_gnss_get_timings( radio_context, &timings );
    if( status != LR11XX_STATUS_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to get gnss timings\n" );
        return false;
    }

    status = lr11xx_gnss_read_used_constellations( radio_context, &constellation_used );
    if( status != LR11XX_STATUS_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to get gnss constellation used\n" );
        return false;
    }

    mw_bsp_get_lr11xx_reg_mode( radio_context, &reg_mode );
    *power_consumption_uah = lr11xx_gnss_get_consumption( reg_mode, timings, constellation_used );

    return true;
}

bool smtc_gnss_is_nav_message_valid( const lr11xx_gnss_constellation_mask_t constellations,
                                     uint8_t                                nb_detected_satellites,
                                     lr11xx_gnss_detected_satellite_t*      detected_satellites )
{
    bool is_valid_nav_message = false;

    /* A NAV message is considered valid if:
        - Dual constellations:
            - at least 2 SVs per constellation (BEIDOU and GNSS only) and 6 SVs in total.
            - 5 SVs in a same constellation.
        - Single constellation: at least 5 detected SVs (BEIDOU and GNSS only).

        GPS satellites ID [0 31]
        SBAS satellites ID [32 63] but not used
        BEIDOU satellites ID [64 127].
    */
    if( nb_detected_satellites >= 5 )
    {
        uint8_t gps_sv_cnt    = 0;
        uint8_t beidou_sv_cnt = 0;

        for( uint8_t i = 0; i < nb_detected_satellites; i++ )
        {
            /* Check if it's a GPS satellite */
            if( detected_satellites[i].satellite_id <= 31 )
            {
                gps_sv_cnt++;
            }
            /* Ignore SBAS */
            /* Check if it's a BEIDOU satellite */
            if( ( detected_satellites[i].satellite_id >= 64 ) && ( detected_satellites[i].satellite_id <= 127 ) )
            {
                beidou_sv_cnt++;
            }
        }

        /* Check if the NAV message is valid */
        if( constellations != ( LR11XX_GNSS_GPS_MASK | LR11XX_GNSS_BEIDOU_MASK ) )
        {
            /* Single constellation */
            if( ( gps_sv_cnt >= 5 ) || ( beidou_sv_cnt >= 5 ) )
            {
                is_valid_nav_message = true;
            }
            else
            {
                is_valid_nav_message = false;
            }
        }
        else
        {
            /* Dual constellations */
            if( ( ( gps_sv_cnt >= 2 ) && ( beidou_sv_cnt >= 2 ) && ( ( gps_sv_cnt + beidou_sv_cnt ) >= 6 ) ) ||
                ( ( gps_sv_cnt >= 5 ) || ( beidou_sv_cnt >= 5 ) ) )
            {
                is_valid_nav_message = true;
            }
            else
            {
                is_valid_nav_message = false;
            }
        }
    }
    else
    {
        is_valid_nav_message = false;
    }

    return is_valid_nav_message;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

static bool gnss_scan( const void* radio_context, lr11xx_gnss_date_t date, const smtc_gnss_scan_params_t* params )
{
    lr11xx_status_t status = LR11XX_STATUS_ERROR;

    status =
        lr11xx_system_set_dio_irq_params( radio_context, LR11XX_SYSTEM_IRQ_GNSS_SCAN_DONE, LR11XX_SYSTEM_IRQ_NONE );
    if( status != LR11XX_STATUS_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to set GNSS scan done IRQ params\n" );
        return false;
    }

    status = lr11xx_gnss_set_scan_mode( radio_context, LR11XX_GNSS_SCAN_MODE_3_SINGLE_SCAN_AND_5_FAST_SCANS );
    if( status != LR11XX_STATUS_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to set GNSS scan mode\n" );
        return false;
    }

    status = lr11xx_gnss_set_constellations_to_use( radio_context, params->constellations );
    if( status != LR11XX_STATUS_OK )
    {
        MW_DBG_TRACE_ERROR( "Failed to set constellations\n" );
        return false;
    }

    if( params->assisted == true )
    {
        status = lr11xx_gnss_set_freq_search_space( radio_context, params->freq_search_space );
        if( status != LR11XX_STATUS_OK )
        {
            MW_DBG_TRACE_ERROR( "Failed to set freq search space\n" );
            return false;
        }

        status = lr11xx_gnss_scan_assisted( radio_context, date, LR11XX_GNSS_OPTION_BEST_EFFORT,
                                            params->input_parameters, params->nb_svs_max );
        if( status != LR11XX_STATUS_OK )
        {
            MW_DBG_TRACE_ERROR( "[GNSS] Failed to start assisted scan\n" );
            return false;
        }
    }
    else
    {
        status = lr11xx_gnss_scan_autonomous( radio_context, date, LR11XX_GNSS_OPTION_BEST_EFFORT,
                                              params->input_parameters, params->nb_svs_max );
        if( status != LR11XX_STATUS_OK )
        {
            MW_DBG_TRACE_ERROR( "[GNSS] Failed to start autonomous scan\n" );
            return false;
        }
    }

    return true;
}

static inline bool gnss_is_result_to_solver( const uint8_t* buffer, uint8_t buffer_length )
{
    if( buffer_length >= 2 )
    {
        return buffer[0] == LR11XX_GNSS_DESTINATION_SOLVER;
    }
    else
    {
        return false;
    }
}

static inline bool gnss_is_result_to_host( const uint8_t* buffer, uint8_t buffer_length )
{
    if( buffer_length >= 2 )
    {
        return buffer[0] == LR11XX_GNSS_DESTINATION_HOST;
    }
    else
    {
        return false;
    }
}

static int median( int n, int x[] )
{
    int temp;
    int i, j;
    // the following two loops sort the array x in ascending order
    for( i = 0; i < n - 1; i++ )
    {
        for( j = i + 1; j < n; j++ )
        {
            if( x[j] < x[i] )
            {
                // swap elements
                temp = x[i];
                x[i] = x[j];
                x[j] = temp;
            }
        }
    }

    if( n % 2 == 0 )
    {
        // if there is an even number of elements, return mean of the two elements in the middle
        return ( ( x[n / 2] + x[n / 2 - 1] ) / 2 );
    }
    else
    {
        // else return the element in the middle
        return x[n / 2];
    }
}

/* --- EOF ------------------------------------------------------------------ */
