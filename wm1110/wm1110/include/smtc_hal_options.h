/*!
 * @file      smtc_hal_options.h
 *
 * @brief     SMTC Hardware abstraction layer OPTIONS management API definition.
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
#ifndef SMTC_HAL_OPTIONS_H
#define SMTC_HAL_OPTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include <stdbool.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

#define HAL_FEATURE_OFF                             0
#define HAL_FEATURE_ON                              !HAL_FEATURE_OFF

#ifndef HAL_DBG_TRACE
#define HAL_DBG_TRACE                               HAL_FEATURE_OFF
#endif // HAL_DBG_TRACE
#define HAL_DBG_TRACE_COLOR                         HAL_FEATURE_OFF

/* HAL_FEATURE_ON to activate sleep mode */

/* HAL_FEATURE_OFF to deactivate sleep mode */
#define HAL_LOW_POWER_MODE                          HAL_FEATURE_ON

/* HAL_FEATURE_ON to enable debug probe, not disallocating corresponding pins */
#define HAL_HW_DEBUG_PROBE                          HAL_FEATURE_OFF

#define HAL_USE_PRINTF_UART                         HAL_FEATURE_OFF
#define HAL_PRINT_BUFFER_SIZE                       255

/* HAL_FEATURE_OFF to not use watchdog */
#define HAL_USE_WATCHDOG                            HAL_FEATURE_OFF

/*!
 * Watchdog counter reload value
 *
 * @remark The period must be lower than MCU watchdog period
 */
#define HAL_WATCHDOG_RELOAD_PERIOD_SECONDS          20

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

#ifdef __cplusplus
}
#endif

#endif  // SMTC_HAL_OPTIONS_H
