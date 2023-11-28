/**
 * @file      gnss_queue.h
 *
 * @brief     Implementation of the GNSS scan group queue.
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

#ifndef GNSS_QUEUE_H
#define GNSS_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include <stdbool.h>

#include "gnss_helpers.h"
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

/**
 * @brief Number of bytes of the metadata field concerning a GNSS scan
 */
#define GNSS_SCAN_METADATA_SIZE 1

/**
 * @brief Maximum size for a MODE3 scan NAV message (10 sv, doppler, no bit change)
 */
#define GNSS_RESULT_SIZE_MAX_MODE3 ( 49 + 1 ) /* 1 byte for destination byte */

/**
 * @brief The minimum number of SV necessary for single NAV position solving
 */
#define GNSS_SCAN_SINGLE_NAV_MIN_SV 6

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/**
 * @brief Description of a scan node in the scan group
 */
typedef struct
{
    uint32_t                         timestamp;     //!< GPS time at which the scan has completed
    uint8_t                          detected_svs;  //!< Number of Space Vehicles detected during the scan
    uint8_t                          results_size;  //!< Size of the result (NAV)
    uint8_t                          results_buffer[GNSS_SCAN_METADATA_SIZE + GNSS_RESULT_SIZE_MAX_MODE3];
    lr11xx_gnss_detected_satellite_t info_svs[GNSS_NB_SVS_MAX];  //!< Information about each SV detected (ID, CNR...)
    bool nav_valid;  //!< Indicates if a single NAV can be used by the solver to get a position */
} gnss_scan_t;

/**
 * @brief Description of a scan group
 */
typedef struct
{
    uint8_t     token;                            //!< Scan group identifier (7-bits roll-over)
    gnss_scan_t scans[GNSS_SCAN_GROUP_SIZE_MAX];  //!< Description of all scans of the group
    uint8_t     scan_group_size;                  //!< Size of the scan group
    uint8_t scan_group_stop;  //!< The number of SVs necessary on a single NAV to close the scan group (0 means no stop)
    uint8_t nb_scans_valid;   //!< Number of valid scans in the group
    uint8_t nb_scans_total;   //!< Total number of scans completed (valid or not)
    uint8_t next_send_index;  //!< Scan index to be sent next
    uint32_t power_consumption_uah;  //!< Power consumption of the complete scan group
    bool     stop;                   //!< Current scan group can be stopped and sent
} gnss_scan_group_queue_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/*!
 * @brief Reset the scan group token
 *
 * @param[in] queue Queue for which the token needs to be reset
 */
void gnss_scan_group_queue_reset_token( gnss_scan_group_queue_t* queue );

/*!
 * @brief Increment the scan group token value (7-bits roll-over)
 *
 * @param[in] queue Queue for which the token needs to be incremented
 */
void gnss_scan_group_queue_increment_token( gnss_scan_group_queue_t* queue );

/*!
 * @brief Reset the scan group queue, except token value
 *
 * @param[in] queue Queue to be initialized and configured
 * @param[in] scan_group_size Size of the scan group
 * @param[in] scan_group_stop Number of SVs to reach to stop current scan group (0: no stop)
 *
 * @return a boolean set to true if the queue could be configured, false otherwise
 */
bool gnss_scan_group_queue_new( gnss_scan_group_queue_t* queue, uint8_t scan_group_size, uint8_t scan_group_stop );

/*!
 * @brief Check if a queue is full
 *
 * A queue is considered full if one of the following confition is met:
 *   - The number of valid GNSS scan result has reached GNSS_SCAN_GROUP_SIZE; or
 *   - The scan group has been aborted due to invalid scan
 *
 * @param[in] queue Queue to check
 *
 * @return a boolean set to true if the queue is full, false otherwise
 */
bool gnss_scan_group_queue_is_full( gnss_scan_group_queue_t* queue );

/*!
 * @brief Check if a scan group is valid
 *
 * Depending on the current scan group mode, a scan group is considered valid if:
 *   - DEFAULT mode: the number of valid GNSS scan result is equal to the group size; or
 *   - SENSITIVITY mode: there is at least 1 valid scan. If there only 1 valid scan, it needs to be a valid NAV message
 * (that the solver can use to get a position)
 *
 * valid scan group: as defined above
 * valid scan      : a successful scan which detected at least 1 SV
 * valid NAV       : a NAV message that can be used by the solver to get a position with this single NAV
 *
 * @param[in] queue Queue to check
 *
 * @return a boolean set to true if the queue is valid, false otherwise
 */
bool gnss_scan_group_queue_is_valid( gnss_scan_group_queue_t* queue );

/*!
 * @brief Add a new scan result to a queue
 * After a call to this function, the user must check if the queue is full with gnss_scan_group_queue_is_full()
 * to avoid overflow.
 *
 * @param[in] queue Queue to update
 * @param[in] scan Scan result to push to the queue
 */
void gnss_scan_group_queue_push( gnss_scan_group_queue_t* queue, gnss_scan_t* scan );

/*!
 * @brief Prepare the scan result payload to be sent over the air, with associated metadata.
 * The format of the payload is: | last NAV (1bit) | RFU (2bits) | token (5bits) | NAV |
 *
 * @param[in] queue Queue from which the result is popped
 * @param[out] buffer Pointer to the prepared buffer ready to be sent over the air
 * @param[out] buffer_size Size of the buffer to be sent
 *
 * @return a boolean set to true is a scan result is ready to be sent, false if there is no result to be sent
 */
bool gnss_scan_group_queue_pop( gnss_scan_group_queue_t* queue, uint8_t** buffer, uint8_t* buffer_size );

#ifdef __cplusplus
}
#endif

#endif  // GNSS_QUEUE_H

/* --- EOF ------------------------------------------------------------------ */
