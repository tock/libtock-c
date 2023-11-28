/*!
 * @file      apps_modem_event.c
 *
 * @brief     LoRa Basics Modem event manager implementation
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

#include "apps_modem_event.h"
#include "smtc_hal_dbg_trace.h"
#include "smtc_modem_api_str.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

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

/*!
 * @brief Lora Basics Modem event callback functions
 */
apps_modem_event_callback_t* apps_modem_event_callback;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

void apps_modem_event_init( apps_modem_event_callback_t* event_callback )
{
    apps_modem_event_callback = event_callback;
}

void apps_modem_event_process( void )
{
    smtc_modem_event_t       current_event;
    smtc_modem_return_code_t return_code = SMTC_MODEM_RC_OK;
    uint8_t                  event_pending_count;
    
    do
    {
        /* Read modem event */
        return_code = smtc_modem_get_event( &current_event, &event_pending_count );

        if( return_code == SMTC_MODEM_RC_OK )
        {
            if( apps_modem_event_callback != NULL )
            {
                switch( current_event.event_type )
                {
                case SMTC_MODEM_EVENT_RESET:
                    HAL_DBG_TRACE_INFO( "###### ===== BASICS MODEM RESET EVENT ==== ######\n" );
                    HAL_DBG_TRACE_PRINTF( "Reset count : %lu \n", current_event.event_data.reset.count );
                    if( apps_modem_event_callback->reset != NULL )
                    {
                        apps_modem_event_callback->reset( current_event.event_data.reset.count );
                    }
                    break;
                case SMTC_MODEM_EVENT_ALARM:
                    HAL_DBG_TRACE_INFO( "###### ===== ALARM EVENT ==== ######\n" );
                    if( apps_modem_event_callback->alarm != NULL )
                    {
                        apps_modem_event_callback->alarm( );
                    }
                    break;
                case SMTC_MODEM_EVENT_JOINED:
                    HAL_DBG_TRACE_INFO( "###### ===== JOINED EVENT ==== ######\n" );
                    if( apps_modem_event_callback->joined != NULL )
                    {
                        apps_modem_event_callback->joined( );
                    }
                    break;
                case SMTC_MODEM_EVENT_JOINFAIL:
                    HAL_DBG_TRACE_INFO( "###### ===== JOINED FAIL EVENT ==== ######\n" );
                    if( apps_modem_event_callback->join_fail != NULL )
                    {
                        apps_modem_event_callback->join_fail( );
                    }
                    break;
                case SMTC_MODEM_EVENT_TXDONE:
                    HAL_DBG_TRACE_INFO( "###### ===== TX DONE EVENT ==== ######\n" );
                    switch( current_event.event_data.txdone.status )
                    {
                    case SMTC_MODEM_EVENT_TXDONE_NOT_SENT:
                        HAL_DBG_TRACE_ERROR( "TX Done status: %s\n", smtc_modem_event_txdone_status_to_str(
                                                                         current_event.event_data.txdone.status ) );
                        break;
                    case SMTC_MODEM_EVENT_TXDONE_SENT:
                    case SMTC_MODEM_EVENT_TXDONE_CONFIRMED:
                    default:
                        HAL_DBG_TRACE_PRINTF( "TX Done status: %s\n", smtc_modem_event_txdone_status_to_str(
                                                                          current_event.event_data.txdone.status ) );
                        break;
                    }
                    if( apps_modem_event_callback->tx_done != NULL )
                    {
                        apps_modem_event_callback->tx_done( current_event.event_data.txdone.status );
                    }
                    break;
                case SMTC_MODEM_EVENT_DOWNDATA:
                    HAL_DBG_TRACE_INFO( "###### ===== DOWNLINK EVENT ==== ######\n" );
                    HAL_DBG_TRACE_PRINTF( "Rx window: %s\n", smtc_modem_event_downdata_window_to_str(
                                                                 current_event.event_data.downdata.window ) );
                    HAL_DBG_TRACE_PRINTF( "Rx port: %d\n", current_event.event_data.downdata.fport );
                    HAL_DBG_TRACE_PRINTF( "Rx RSSI: %d\n", current_event.event_data.downdata.rssi - 64 );
                    HAL_DBG_TRACE_PRINTF( "Rx SNR: %d\n", current_event.event_data.downdata.snr / 4 );

                    if( apps_modem_event_callback->down_data != NULL )
                    {
                        apps_modem_event_callback->down_data(
                            current_event.event_data.downdata.rssi, current_event.event_data.downdata.snr,
                            current_event.event_data.downdata.window, current_event.event_data.downdata.fport,
                            current_event.event_data.downdata.data, current_event.event_data.downdata.length );
                    }
                    break;
                case SMTC_MODEM_EVENT_UPLOADDONE:
                    HAL_DBG_TRACE_INFO( "###### ===== UPLOAD DONE EVENT ==== ######\n" );
                    HAL_DBG_TRACE_PRINTF( "Upload status: %s\n", smtc_modem_event_uploaddone_status_to_str(
                                                                     current_event.event_data.uploaddone.status ) );
                    if( apps_modem_event_callback->upload_done != NULL )
                    {
                        apps_modem_event_callback->upload_done( current_event.event_data.uploaddone.status );
                    }
                    break;
                case SMTC_MODEM_EVENT_SETCONF:
                    HAL_DBG_TRACE_INFO( "###### ===== SET CONF EVENT ==== ######\n" );
                    HAL_DBG_TRACE_PRINTF( "Tag: %s",
                                          smtc_modem_event_setconf_tag_to_str( current_event.event_data.setconf.tag ) );
                    if( apps_modem_event_callback->set_conf != NULL )
                    {
                        apps_modem_event_callback->set_conf( current_event.event_data.setconf.tag );
                    }
                    break;
                case SMTC_MODEM_EVENT_MUTE:
                    HAL_DBG_TRACE_INFO( "###### ===== MUTE EVENT ==== ######\n" );
                    HAL_DBG_TRACE_PRINTF( "Mute: %s\n",
                                          smtc_modem_event_mute_status_to_str( current_event.event_data.mute.status ) );
                    if( apps_modem_event_callback->mute != NULL )
                    {
                        apps_modem_event_callback->mute( current_event.event_data.mute.status );
                    }
                    break;
                case SMTC_MODEM_EVENT_STREAMDONE:
                    HAL_DBG_TRACE_INFO( "###### ===== STREAM DONE EVENT ==== ######\n" );
                    if( apps_modem_event_callback->stream_done != NULL )
                    {
                        apps_modem_event_callback->stream_done( );
                    }
                    break;
                case SMTC_MODEM_EVENT_TIME:
                    HAL_DBG_TRACE_INFO( "###### ===== TIME EVENT ==== ######\n" );
                    HAL_DBG_TRACE_PRINTF( "Time: %s\n",
                                          smtc_modem_event_time_status_to_str( current_event.event_data.time.status ) );
                    if( apps_modem_event_callback->time_updated_alc_sync != NULL )
                    {
                        apps_modem_event_callback->time_updated_alc_sync( current_event.event_data.time.status );
                    }
                    break;
                case SMTC_MODEM_EVENT_TIMEOUT_ADR_CHANGED:
                    HAL_DBG_TRACE_INFO( "###### ===== ADR CHANGED EVENT ==== ######\n" );
                    if( apps_modem_event_callback->adr_mobile_to_static != NULL )
                    {
                        apps_modem_event_callback->adr_mobile_to_static( );
                    }
                    break;
                case SMTC_MODEM_EVENT_NEW_LINK_ADR:
                    HAL_DBG_TRACE_INFO( "###### ===== NEW LINK ADR EVENT ==== ######\n" );
                    if( apps_modem_event_callback->new_link_adr != NULL )
                    {
                        apps_modem_event_callback->new_link_adr( );
                    }
                    break;
                case SMTC_MODEM_EVENT_LINK_CHECK:
                    HAL_DBG_TRACE_INFO( "###### ===== LINK CHECK EVENT ==== ######\n" );
                    HAL_DBG_TRACE_PRINTF( "Link status: %s\n", smtc_modem_event_link_check_status_to_str(
                                                                   current_event.event_data.link_check.status ) );
                    HAL_DBG_TRACE_PRINTF( "Margin: %d dB\n", current_event.event_data.link_check.margin );
                    HAL_DBG_TRACE_PRINTF( "Number of gateways: %d\n", current_event.event_data.link_check.gw_cnt );
                    if( apps_modem_event_callback->link_status != NULL )
                    {
                        apps_modem_event_callback->link_status( current_event.event_data.link_check.status,
                                                                current_event.event_data.link_check.margin,
                                                                current_event.event_data.link_check.gw_cnt );
                    }
                    break;
                case SMTC_MODEM_EVENT_ALMANAC_UPDATE:
                    HAL_DBG_TRACE_INFO( "###### ===== ALMANAC UPDATE EVENT ==== ######\n" );
                    HAL_DBG_TRACE_PRINTF( "Almanac update status: %s\n",
                                          smtc_modem_event_almanac_update_status_to_str(
                                              current_event.event_data.almanac_update.status ) );
                    if( apps_modem_event_callback->almanac_update != NULL )
                    {
                        apps_modem_event_callback->almanac_update( current_event.event_data.almanac_update.status );
                    }
                    break;
                case SMTC_MODEM_EVENT_USER_RADIO_ACCESS:
                    HAL_DBG_TRACE_INFO( "###### ===== USER RADIO ACCESS EVENT ==== ######\n" );
                    if( apps_modem_event_callback->user_radio_access != NULL )
                    {
                        apps_modem_event_callback->user_radio_access(
                            current_event.event_data.user_radio_access.timestamp_ms,
                            current_event.event_data.user_radio_access.status );
                    }
                    break;
                case SMTC_MODEM_EVENT_CLASS_B_PING_SLOT_INFO:
                    HAL_DBG_TRACE_INFO( "###### ===== CLASS B PING SLOT INFO EVENT ==== ######\n" );
                    HAL_DBG_TRACE_PRINTF( "Class B ping slot status: %s\n",
                                          smtc_modem_event_class_b_ping_slot_status_to_str(
                                              current_event.event_data.class_b_ping_slot_info.status ) );
                    if( apps_modem_event_callback->class_b_ping_slot_info != NULL )
                    {
                        apps_modem_event_callback->class_b_ping_slot_info(
                            current_event.event_data.class_b_ping_slot_info.status );
                    }
                    break;
                case SMTC_MODEM_EVENT_CLASS_B_STATUS:
                    HAL_DBG_TRACE_INFO( "###### ===== CLASS B STATUS EVENT ==== ######\n" );
                    HAL_DBG_TRACE_PRINTF(
                        "Class B status: %s\n",
                        smtc_modem_event_class_b_status_to_str( current_event.event_data.class_b_status.status ) );
                    if( apps_modem_event_callback->class_b_status != NULL )
                    {
                        apps_modem_event_callback->class_b_status( current_event.event_data.class_b_status.status );
                    }
                    break;
                case SMTC_MODEM_EVENT_MIDDLEWARE_1:
                    HAL_DBG_TRACE_INFO( "###### ===== MIDDLEWARE_1 EVENT ==== ######\n" );
                    if( apps_modem_event_callback->middleware_1 != NULL )
                    {
                        apps_modem_event_callback->middleware_1(
                            current_event.event_data.middleware_event_status.status );
                    }
                    break;
                case SMTC_MODEM_EVENT_MIDDLEWARE_2:
                    HAL_DBG_TRACE_INFO( "###### ===== MIDDLEWARE_2 EVENT ==== ######\n" );
                    if( apps_modem_event_callback->middleware_2 != NULL )
                    {
                        apps_modem_event_callback->middleware_2(
                            current_event.event_data.middleware_event_status.status );
                    }
                    break;
                case SMTC_MODEM_EVENT_MIDDLEWARE_3:
                    HAL_DBG_TRACE_INFO( "###### ===== MIDDLEWARE_3 EVENT ==== ######\n" );
                    if( apps_modem_event_callback->middleware_3 != NULL )
                    {
                        apps_modem_event_callback->middleware_3(
                            current_event.event_data.middleware_event_status.status );
                    }
                    break;
                case SMTC_MODEM_EVENT_NONE:
                    break;
                default:
                    HAL_DBG_TRACE_INFO( "###### ===== UNKNOWN EVENT %u ==== ######\n", current_event.event_type );
                    break;
                }
            }
            else
            {
                HAL_DBG_TRACE_ERROR( "lora_basics_modem_event_callback not defined\n" );
            }
        }
        else
        {
            HAL_DBG_TRACE_ERROR( "smtc_modem_get_event != SMTC_MODEM_RC_OK\n" );
        }
    } while( ( return_code == SMTC_MODEM_RC_OK ) && ( current_event.event_type != SMTC_MODEM_EVENT_NONE ) );
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/* --- EOF ------------------------------------------------------------------ */
