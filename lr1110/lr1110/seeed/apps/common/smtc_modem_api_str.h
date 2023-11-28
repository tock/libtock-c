/*!
 * @file      smtc_modem_api_str.h
 *
 * @brief     String helper functions for SMTC modem API
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
 
 #ifndef SMTC_MODEM_API_STR_H
#define SMTC_MODEM_API_STR_H
#include "smtc_modem_api.h"
#ifdef __cplusplus
extern "C" {
#endif
const char *smtc_modem_return_code_to_str(const smtc_modem_return_code_t value);
const char *smtc_modem_adr_profile_to_str(const smtc_modem_adr_profile_t value);
const char *smtc_modem_class_to_str(const smtc_modem_class_t value);
const char *smtc_modem_file_upload_cipher_mode_to_str(const smtc_modem_file_upload_cipher_mode_t value);
const char *smtc_modem_stream_cipher_mode_to_str(const smtc_modem_stream_cipher_mode_t value);
const char *smtc_modem_dm_info_interval_format_to_str(const smtc_modem_dm_info_interval_format_t value);
const char *smtc_modem_region_to_str(const smtc_modem_region_t value);
const char *smtc_modem_mc_grp_id_to_str(const smtc_modem_mc_grp_id_t value);
const char *smtc_modem_stack_state_to_str(const smtc_modem_stack_state_t value);
const char *smtc_modem_event_downdata_window_to_str(const smtc_modem_event_downdata_window_t value);
const char *smtc_modem_time_sync_service_to_str(const smtc_modem_time_sync_service_t value);
const char *smtc_modem_event_time_status_to_str(const smtc_modem_event_time_status_t value);
const char *smtc_modem_event_link_check_status_to_str(const smtc_modem_event_link_check_status_t value);
const char *smtc_modem_event_txdone_status_to_str(const smtc_modem_event_txdone_status_t value);
const char *smtc_modem_event_mute_status_to_str(const smtc_modem_event_mute_status_t value);
const char *smtc_modem_event_uploaddone_status_to_str(const smtc_modem_event_uploaddone_status_t value);
const char *smtc_modem_event_setconf_tag_to_str(const smtc_modem_event_setconf_tag_t value);
const char *smtc_modem_event_almanac_update_status_to_str(const smtc_modem_event_almanac_update_status_t value);
const char *smtc_modem_event_class_b_status_to_str(const smtc_modem_event_class_b_status_t value);
const char *smtc_modem_event_class_b_ping_slot_status_to_str(const smtc_modem_event_class_b_ping_slot_status_t value);
const char *smtc_modem_event_user_radio_access_status_to_str(const smtc_modem_event_user_radio_access_status_t value);
const char *smtc_modem_class_b_ping_slot_periodicity_to_str(const smtc_modem_class_b_ping_slot_periodicity_t value);
const char *smtc_modem_frame_pending_bit_status_to_str(const smtc_modem_frame_pending_bit_status_t value);
const char *smtc_modem_d2d_class_b_tx_done_status_to_str(const smtc_modem_d2d_class_b_tx_done_status_t value);
#ifdef __cplusplus
}
#endif
#endif // SMTC_MODEM_API_STR_H
