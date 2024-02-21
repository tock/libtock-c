/*!
 * @file      apps_utilities.h
 *
 * @brief     Common Application Helper functions
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
#ifndef APPS_UTILITIES_H
#define APPS_UTILITIES_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include "smtc_modem_api.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*!
 * @brief Stringify constants
 */
#define xstr( a ) str( a )
#define str( a ) #a

/*!
 * @brief Helper macro that returned a human-friendly message if a command does not return SMTC_MODEM_RC_OK
 *
 * @remark The macro is implemented to be used with functions returning a @ref smtc_modem_return_code_t
 *
 * @param[in] rc  Return code
 */
#define ASSERT_SMTC_MODEM_RC( rc )                                                           \
                                                                                             \
    if( rc != SMTC_MODEM_RC_OK )                                                             \
    {                                                                                        \
        if( rc == SMTC_MODEM_RC_NOT_INIT )                                                   \
        {                                                                                    \
            HAL_DBG_TRACE_ERROR( "In %s - %s (line %d): %s\n", __FILE__, __func__, __LINE__, \
                                 xstr( SMTC_MODEM_RC_NOT_INIT ) );                           \
        }                                                                                    \
        else if( rc == SMTC_MODEM_RC_INVALID )                                               \
        {                                                                                    \
            HAL_DBG_TRACE_ERROR( "In %s - %s (line %d): %s\n", __FILE__, __func__, __LINE__, \
                                 xstr( SMTC_MODEM_RC_INVALID ) );                            \
        }                                                                                    \
        else if( rc == SMTC_MODEM_RC_BUSY )                                                  \
        {                                                                                    \
            HAL_DBG_TRACE_ERROR( "In %s - %s (line %d): %s\n", __FILE__, __func__, __LINE__, \
                                 xstr( SMTC_MODEM_RC_BUSY ) );                               \
        }                                                                                    \
        else if( rc == SMTC_MODEM_RC_FAIL )                                                  \
        {                                                                                    \
            HAL_DBG_TRACE_ERROR( "In %s - %s (line %d): %s\n", __FILE__, __func__, __LINE__, \
                                 xstr( SMTC_MODEM_RC_FAIL ) );                               \
        }                                                                                    \
        else if( rc == SMTC_MODEM_RC_BAD_SIZE )                                              \
        {                                                                                    \
            HAL_DBG_TRACE_ERROR( "In %s - %s (line %d): %s\n", __FILE__, __func__, __LINE__, \
                                 xstr( SMTC_MODEM_RC_BAD_SIZE ) );                           \
        }                                                                                    \
        else if( rc == SMTC_MODEM_RC_NO_TIME )                                               \
        {                                                                                    \
            HAL_DBG_TRACE_ERROR( "In %s - %s (line %d): %s\n", __FILE__, __func__, __LINE__, \
                                 xstr( SMTC_MODEM_RC_NO_TIME ) );                            \
        }                                                                                    \
        else if( rc == SMTC_MODEM_RC_INVALID_STACK_ID )                                      \
        {                                                                                    \
            HAL_DBG_TRACE_ERROR( "In %s - %s (line %d): %s\n", __FILE__, __func__, __LINE__, \
                                 xstr( SMTC_MODEM_RC_INVALID_STACK_ID ) );                   \
        }                                                                                    \
    }

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/*!
 * @brief convert modem status to string
 */
void modem_status_to_string( smtc_modem_status_mask_t modem_status );

/*!
 * @brief convert lorawan class status to string
 */
void modem_class_to_string( smtc_modem_class_t lorawan_class );

/*!
 * @brief convert lorawan region status to string
 */
void modem_region_to_string( smtc_modem_region_t region );

#ifdef __cplusplus
}
#endif

#endif  // APPS_UTILITIES_H

/* --- EOF ------------------------------------------------------------------ */
