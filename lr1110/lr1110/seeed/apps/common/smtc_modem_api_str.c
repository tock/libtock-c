/*!
 * @file      smtc_modem_api_str.c
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
 
 #include "smtc_modem_api_str.h"

const char *smtc_modem_return_code_to_str(const smtc_modem_return_code_t value)
{
  switch (value)
  {
    case SMTC_MODEM_RC_OK:
    {
      return (const char *) "SMTC_MODEM_RC_OK";
    }

    case SMTC_MODEM_RC_NOT_INIT:
    {
      return (const char *) "SMTC_MODEM_RC_NOT_INIT";
    }

    case SMTC_MODEM_RC_INVALID:
    {
      return (const char *) "SMTC_MODEM_RC_INVALID";
    }

    case SMTC_MODEM_RC_BUSY:
    {
      return (const char *) "SMTC_MODEM_RC_BUSY";
    }

    case SMTC_MODEM_RC_FAIL:
    {
      return (const char *) "SMTC_MODEM_RC_FAIL";
    }

    case SMTC_MODEM_RC_BAD_SIZE:
    {
      return (const char *) "SMTC_MODEM_RC_BAD_SIZE";
    }

    case SMTC_MODEM_RC_MODEM_E_FRAME_ERROR:
    {
      return (const char *) "SMTC_MODEM_RC_MODEM_E_FRAME_ERROR";
    }

    case SMTC_MODEM_RC_NO_TIME:
    {
      return (const char *) "SMTC_MODEM_RC_NO_TIME";
    }

    case SMTC_MODEM_RC_INVALID_STACK_ID:
    {
      return (const char *) "SMTC_MODEM_RC_INVALID_STACK_ID";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_adr_profile_to_str(const smtc_modem_adr_profile_t value)
{
  switch (value)
  {
    case SMTC_MODEM_ADR_PROFILE_NETWORK_CONTROLLED:
    {
      return (const char *) "SMTC_MODEM_ADR_PROFILE_NETWORK_CONTROLLED";
    }

    case SMTC_MODEM_ADR_PROFILE_MOBILE_LONG_RANGE:
    {
      return (const char *) "SMTC_MODEM_ADR_PROFILE_MOBILE_LONG_RANGE";
    }

    case SMTC_MODEM_ADR_PROFILE_MOBILE_LOW_POWER:
    {
      return (const char *) "SMTC_MODEM_ADR_PROFILE_MOBILE_LOW_POWER";
    }

    case SMTC_MODEM_ADR_PROFILE_CUSTOM:
    {
      return (const char *) "SMTC_MODEM_ADR_PROFILE_CUSTOM";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_class_to_str(const smtc_modem_class_t value)
{
  switch (value)
  {
    case SMTC_MODEM_CLASS_A:
    {
      return (const char *) "SMTC_MODEM_CLASS_A";
    }

    case SMTC_MODEM_CLASS_B:
    {
      return (const char *) "SMTC_MODEM_CLASS_B";
    }

    case SMTC_MODEM_CLASS_C:
    {
      return (const char *) "SMTC_MODEM_CLASS_C";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_file_upload_cipher_mode_to_str(const smtc_modem_file_upload_cipher_mode_t value)
{
  switch (value)
  {
    case SMTC_MODEM_FILE_UPLOAD_NO_CIPHER:
    {
      return (const char *) "SMTC_MODEM_FILE_UPLOAD_NO_CIPHER";
    }

    case SMTC_MODEM_FILE_UPLOAD_AES_WITH_APPSKEY:
    {
      return (const char *) "SMTC_MODEM_FILE_UPLOAD_AES_WITH_APPSKEY";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_stream_cipher_mode_to_str(const smtc_modem_stream_cipher_mode_t value)
{
  switch (value)
  {
    case SMTC_MODEM_STREAM_NO_CIPHER:
    {
      return (const char *) "SMTC_MODEM_STREAM_NO_CIPHER";
    }

    case SMTC_MODEM_STREAM_AES_WITH_APPSKEY:
    {
      return (const char *) "SMTC_MODEM_STREAM_AES_WITH_APPSKEY";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_dm_info_interval_format_to_str(const smtc_modem_dm_info_interval_format_t value)
{
  switch (value)
  {
    case SMTC_MODEM_DM_INFO_INTERVAL_IN_SECOND:
    {
      return (const char *) "SMTC_MODEM_DM_INFO_INTERVAL_IN_SECOND";
    }

    case SMTC_MODEM_DM_INFO_INTERVAL_IN_DAY:
    {
      return (const char *) "SMTC_MODEM_DM_INFO_INTERVAL_IN_DAY";
    }

    case SMTC_MODEM_DM_INFO_INTERVAL_IN_HOUR:
    {
      return (const char *) "SMTC_MODEM_DM_INFO_INTERVAL_IN_HOUR";
    }

    case SMTC_MODEM_DM_INFO_INTERVAL_IN_MINUTE:
    {
      return (const char *) "SMTC_MODEM_DM_INFO_INTERVAL_IN_MINUTE";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_region_to_str(const smtc_modem_region_t value)
{
  switch (value)
  {
    case SMTC_MODEM_REGION_EU_868:
    {
      return (const char *) "SMTC_MODEM_REGION_EU_868";
    }

    case SMTC_MODEM_REGION_AS_923_GRP1:
    {
      return (const char *) "SMTC_MODEM_REGION_AS_923_GRP1";
    }

    case SMTC_MODEM_REGION_US_915:
    {
      return (const char *) "SMTC_MODEM_REGION_US_915";
    }

    case SMTC_MODEM_REGION_AU_915:
    {
      return (const char *) "SMTC_MODEM_REGION_AU_915";
    }

    case SMTC_MODEM_REGION_CN_470:
    {
      return (const char *) "SMTC_MODEM_REGION_CN_470";
    }

    case SMTC_MODEM_REGION_WW2G4:
    {
      return (const char *) "SMTC_MODEM_REGION_WW2G4";
    }

    case SMTC_MODEM_REGION_AS_923_GRP2:
    {
      return (const char *) "SMTC_MODEM_REGION_AS_923_GRP2";
    }

    case SMTC_MODEM_REGION_AS_923_GRP3:
    {
      return (const char *) "SMTC_MODEM_REGION_AS_923_GRP3";
    }

    case SMTC_MODEM_REGION_IN_865:
    {
      return (const char *) "SMTC_MODEM_REGION_IN_865";
    }

    case SMTC_MODEM_REGION_KR_920:
    {
      return (const char *) "SMTC_MODEM_REGION_KR_920";
    }

    case SMTC_MODEM_REGION_RU_864:
    {
      return (const char *) "SMTC_MODEM_REGION_RU_864";
    }

    case SMTC_MODEM_REGION_CN_470_RP_1_0:
    {
      return (const char *) "SMTC_MODEM_REGION_CN_470_RP_1_0";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_mc_grp_id_to_str(const smtc_modem_mc_grp_id_t value)
{
  switch (value)
  {
    case SMTC_MODEM_MC_GRP_0:
    {
      return (const char *) "SMTC_MODEM_MC_GRP_0";
    }

    case SMTC_MODEM_MC_GRP_1:
    {
      return (const char *) "SMTC_MODEM_MC_GRP_1";
    }

    case SMTC_MODEM_MC_GRP_2:
    {
      return (const char *) "SMTC_MODEM_MC_GRP_2";
    }

    case SMTC_MODEM_MC_GRP_3:
    {
      return (const char *) "SMTC_MODEM_MC_GRP_3";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_stack_state_to_str(const smtc_modem_stack_state_t value)
{
  switch (value)
  {
    case SMTC_MODEM_STACK_STATE_IDLE:
    {
      return (const char *) "SMTC_MODEM_STACK_STATE_IDLE";
    }

    case SMTC_MODEM_STACK_STATE_BUSY:
    {
      return (const char *) "SMTC_MODEM_STACK_STATE_BUSY";
    }

    case SMTC_MODEM_STACK_STATE_TX_WAIT:
    {
      return (const char *) "SMTC_MODEM_STACK_STATE_TX_WAIT";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_event_downdata_window_to_str(const smtc_modem_event_downdata_window_t value)
{
  switch (value)
  {
    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RX1:
    {
      return (const char *) "SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RX1";
    }

    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RX2:
    {
      return (const char *) "SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RX2";
    }

    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXC:
    {
      return (const char *) "SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXC";
    }

    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXC_MC_GRP0:
    {
      return (const char *) "SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXC_MC_GRP0";
    }

    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXC_MC_GRP1:
    {
      return (const char *) "SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXC_MC_GRP1";
    }

    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXC_MC_GRP2:
    {
      return (const char *) "SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXC_MC_GRP2";
    }

    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXC_MC_GRP3:
    {
      return (const char *) "SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXC_MC_GRP3";
    }

    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXB:
    {
      return (const char *) "SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXB";
    }

    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXB_MC_GRP0:
    {
      return (const char *) "SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXB_MC_GRP0";
    }

    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXB_MC_GRP1:
    {
      return (const char *) "SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXB_MC_GRP1";
    }

    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXB_MC_GRP2:
    {
      return (const char *) "SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXB_MC_GRP2";
    }

    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXB_MC_GRP3:
    {
      return (const char *) "SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXB_MC_GRP3";
    }

    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXBEACON:
    {
      return (const char *) "SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXBEACON";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_time_sync_service_to_str(const smtc_modem_time_sync_service_t value)
{
  switch (value)
  {
    case SMTC_MODEM_TIME_MAC_SYNC:
    {
      return (const char *) "SMTC_MODEM_TIME_MAC_SYNC";
    }

    case SMTC_MODEM_TIME_ALC_SYNC:
    {
      return (const char *) "SMTC_MODEM_TIME_ALC_SYNC";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_event_time_status_to_str(const smtc_modem_event_time_status_t value)
{
  switch (value)
  {
    case SMTC_MODEM_EVENT_TIME_NOT_VALID:
    {
      return (const char *) "SMTC_MODEM_EVENT_TIME_NOT_VALID";
    }

    case SMTC_MODEM_EVENT_TIME_VALID:
    {
      return (const char *) "SMTC_MODEM_EVENT_TIME_VALID";
    }

    case SMTC_MODEM_EVENT_TIME_VALID_BUT_NOT_SYNC:
    {
      return (const char *) "SMTC_MODEM_EVENT_TIME_VALID_BUT_NOT_SYNC";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_event_link_check_status_to_str(const smtc_modem_event_link_check_status_t value)
{
  switch (value)
  {
    case SMTC_MODEM_EVENT_LINK_CHECK_NOT_RECEIVED:
    {
      return (const char *) "SMTC_MODEM_EVENT_LINK_CHECK_NOT_RECEIVED";
    }

    case SMTC_MODEM_EVENT_LINK_CHECK_RECEIVED:
    {
      return (const char *) "SMTC_MODEM_EVENT_LINK_CHECK_RECEIVED";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_event_txdone_status_to_str(const smtc_modem_event_txdone_status_t value)
{
  switch (value)
  {
    case SMTC_MODEM_EVENT_TXDONE_NOT_SENT:
    {
      return (const char *) "SMTC_MODEM_EVENT_TXDONE_NOT_SENT";
    }

    case SMTC_MODEM_EVENT_TXDONE_SENT:
    {
      return (const char *) "SMTC_MODEM_EVENT_TXDONE_SENT";
    }

    case SMTC_MODEM_EVENT_TXDONE_CONFIRMED:
    {
      return (const char *) "SMTC_MODEM_EVENT_TXDONE_CONFIRMED";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_event_mute_status_to_str(const smtc_modem_event_mute_status_t value)
{
  switch (value)
  {
    case SMTC_MODEM_EVENT_MUTE_OFF:
    {
      return (const char *) "SMTC_MODEM_EVENT_MUTE_OFF";
    }

    case SMTC_MODEM_EVENT_MUTE_ON:
    {
      return (const char *) "SMTC_MODEM_EVENT_MUTE_ON";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_event_uploaddone_status_to_str(const smtc_modem_event_uploaddone_status_t value)
{
  switch (value)
  {
    case SMTC_MODEM_EVENT_UPLOADDONE_ABORTED:
    {
      return (const char *) "SMTC_MODEM_EVENT_UPLOADDONE_ABORTED";
    }

    case SMTC_MODEM_EVENT_UPLOADDONE_SUCCESSFUL:
    {
      return (const char *) "SMTC_MODEM_EVENT_UPLOADDONE_SUCCESSFUL";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_event_setconf_tag_to_str(const smtc_modem_event_setconf_tag_t value)
{
  switch (value)
  {
    case SMTC_MODEM_EVENT_SETCONF_ADR_MODE_UPDATED:
    {
      return (const char *) "SMTC_MODEM_EVENT_SETCONF_ADR_MODE_UPDATED";
    }

    case SMTC_MODEM_EVENT_SETCONF_JOIN_EUI_UPDATED:
    {
      return (const char *) "SMTC_MODEM_EVENT_SETCONF_JOIN_EUI_UPDATED";
    }

    case SMTC_MODEM_EVENT_SETCONF_DM_INTERVAL_UPDATED:
    {
      return (const char *) "SMTC_MODEM_EVENT_SETCONF_DM_INTERVAL_UPDATED";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_event_almanac_update_status_to_str(const smtc_modem_event_almanac_update_status_t value)
{
  switch (value)
  {
    case SMTC_MODEM_EVENT_ALMANAC_UPDATE_COMPLETED:
    {
      return (const char *) "SMTC_MODEM_EVENT_ALMANAC_UPDATE_COMPLETED";
    }

    case SMTC_MODEM_EVENT_ALMANAC_UPDATE_STATUS_REQUESTED:
    {
      return (const char *) "SMTC_MODEM_EVENT_ALMANAC_UPDATE_STATUS_REQUESTED";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_event_class_b_status_to_str(const smtc_modem_event_class_b_status_t value)
{
  switch (value)
  {
    case SMTC_MODEM_EVENT_CLASS_B_NOT_READY:
    {
      return (const char *) "SMTC_MODEM_EVENT_CLASS_B_NOT_READY";
    }

    case SMTC_MODEM_EVENT_CLASS_B_READY:
    {
      return (const char *) "SMTC_MODEM_EVENT_CLASS_B_READY";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_event_class_b_ping_slot_status_to_str(const smtc_modem_event_class_b_ping_slot_status_t value)
{
  switch (value)
  {
    case SMTC_MODEM_EVENT_CLASS_B_PING_SLOT_NOT_ANSWERED:
    {
      return (const char *) "SMTC_MODEM_EVENT_CLASS_B_PING_SLOT_NOT_ANSWERED";
    }

    case SMTC_MODEM_EVENT_CLASS_B_PING_SLOT_ANSWERED:
    {
      return (const char *) "SMTC_MODEM_EVENT_CLASS_B_PING_SLOT_ANSWERED";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_event_user_radio_access_status_to_str(const smtc_modem_event_user_radio_access_status_t value)
{
  switch (value)
  {
    case SMTC_MODEM_EVENT_USER_RADIO_ACCESS_RX_ERROR:
    {
      return (const char *) "SMTC_MODEM_EVENT_USER_RADIO_ACCESS_RX_ERROR";
    }

    case SMTC_MODEM_EVENT_USER_RADIO_ACCESS_CAD_OK:
    {
      return (const char *) "SMTC_MODEM_EVENT_USER_RADIO_ACCESS_CAD_OK";
    }

    case SMTC_MODEM_EVENT_USER_RADIO_ACCESS_CAD_DONE:
    {
      return (const char *) "SMTC_MODEM_EVENT_USER_RADIO_ACCESS_CAD_DONE";
    }

    case SMTC_MODEM_EVENT_USER_RADIO_ACCESS_TX_DONE:
    {
      return (const char *) "SMTC_MODEM_EVENT_USER_RADIO_ACCESS_TX_DONE";
    }

    case SMTC_MODEM_EVENT_USER_RADIO_ACCESS_RX_DONE:
    {
      return (const char *) "SMTC_MODEM_EVENT_USER_RADIO_ACCESS_RX_DONE";
    }

    case SMTC_MODEM_EVENT_USER_RADIO_ACCESS_RX_TIMEOUT:
    {
      return (const char *) "SMTC_MODEM_EVENT_USER_RADIO_ACCESS_RX_TIMEOUT";
    }

    case SMTC_MODEM_EVENT_USER_RADIO_ACCESS_WIFI_SCAN_DONE:
    {
      return (const char *) "SMTC_MODEM_EVENT_USER_RADIO_ACCESS_WIFI_SCAN_DONE";
    }

    case SMTC_MODEM_EVENT_USER_RADIO_ACCESS_GNSS_SCAN_DONE:
    {
      return (const char *) "SMTC_MODEM_EVENT_USER_RADIO_ACCESS_GNSS_SCAN_DONE";
    }

    case SMTC_MODEM_EVENT_USER_RADIO_ACCESS_ABORTED:
    {
      return (const char *) "SMTC_MODEM_EVENT_USER_RADIO_ACCESS_ABORTED";
    }

    case SMTC_MODEM_EVENT_USER_RADIO_ACCESS_UNKNOWN:
    {
      return (const char *) "SMTC_MODEM_EVENT_USER_RADIO_ACCESS_UNKNOWN";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_class_b_ping_slot_periodicity_to_str(const smtc_modem_class_b_ping_slot_periodicity_t value)
{
  switch (value)
  {
    case SMTC_MODEM_CLASS_B_PINGSLOT_1_S:
    {
      return (const char *) "SMTC_MODEM_CLASS_B_PINGSLOT_1_S";
    }

    case SMTC_MODEM_CLASS_B_PINGSLOT_2_S:
    {
      return (const char *) "SMTC_MODEM_CLASS_B_PINGSLOT_2_S";
    }

    case SMTC_MODEM_CLASS_B_PINGSLOT_4_S:
    {
      return (const char *) "SMTC_MODEM_CLASS_B_PINGSLOT_4_S";
    }

    case SMTC_MODEM_CLASS_B_PINGSLOT_8_S:
    {
      return (const char *) "SMTC_MODEM_CLASS_B_PINGSLOT_8_S";
    }

    case SMTC_MODEM_CLASS_B_PINGSLOT_16_S:
    {
      return (const char *) "SMTC_MODEM_CLASS_B_PINGSLOT_16_S";
    }

    case SMTC_MODEM_CLASS_B_PINGSLOT_32_S:
    {
      return (const char *) "SMTC_MODEM_CLASS_B_PINGSLOT_32_S";
    }

    case SMTC_MODEM_CLASS_B_PINGSLOT_64_S:
    {
      return (const char *) "SMTC_MODEM_CLASS_B_PINGSLOT_64_S";
    }

    case SMTC_MODEM_CLASS_B_PINGSLOT_128_S:
    {
      return (const char *) "SMTC_MODEM_CLASS_B_PINGSLOT_128_S";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_frame_pending_bit_status_to_str(const smtc_modem_frame_pending_bit_status_t value)
{
  switch (value)
  {
    case SMTC_MODEM_NO_DATA_ARE_PENDING:
    {
      return (const char *) "SMTC_MODEM_NO_DATA_ARE_PENDING";
    }

    case SMTC_MODEM_DATA_ARE_PENDING:
    {
      return (const char *) "SMTC_MODEM_DATA_ARE_PENDING";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

const char *smtc_modem_d2d_class_b_tx_done_status_to_str(const smtc_modem_d2d_class_b_tx_done_status_t value)
{
  switch (value)
  {
    case SMTC_MODEM_EVENT_D2D_CLASS_B_TX_DONE_NOT_SENT:
    {
      return (const char *) "SMTC_MODEM_EVENT_D2D_CLASS_B_TX_DONE_NOT_SENT";
    }

    case SMTC_MODEM_EVENT_D2D_CLASS_B_TX_DONE_SENT:
    {
      return (const char *) "SMTC_MODEM_EVENT_D2D_CLASS_B_TX_DONE_SENT";
    }

    default:
    {
      return (const char *) "Unknown";
    }

  }

}

