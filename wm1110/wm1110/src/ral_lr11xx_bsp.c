/*!
 * \file      ral_lr11xx_bsp.c
 *
 * \brief     Implements the BSP (BoardSpecificPackage) HAL functions for LR11XX
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

#include <stdint.h>   // C99 types
#include <stdbool.h>  // bool type

#include "ral_lr11xx_bsp.h"
#include "smtc_hal_mcu.h"
#include "smtc_board.h"
#include "smtc_shield_lr11xx_common_if.h"

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

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

void ral_lr11xx_bsp_get_rf_switch_cfg( const void* context, lr11xx_system_rfswitch_cfg_t* rf_switch_cfg )
{
    *rf_switch_cfg = smtc_shield_lr11xx_get_rf_switch_cfg( );
}

void ral_lr11xx_bsp_get_tx_cfg( const void* context, const ral_lr11xx_bsp_tx_cfg_input_params_t* input_params,
                                ral_lr11xx_bsp_tx_cfg_output_params_t* output_params )
{
    const int8_t modem_tx_offset = smtc_board_get_tx_power_offset( );

    const smtc_shield_lr11xx_pa_pwr_cfg_t* pa_pwr_cfg = smtc_shield_lr11xx_get_pa_pwr_cfg(
        input_params->freq_in_hz, input_params->system_output_pwr_in_dbm + modem_tx_offset );

    output_params->chip_output_pwr_in_dbm_expected   = input_params->system_output_pwr_in_dbm + modem_tx_offset;
    output_params->chip_output_pwr_in_dbm_configured = pa_pwr_cfg->power;

    output_params->pa_cfg.pa_duty_cycle = pa_pwr_cfg->pa_config.pa_duty_cycle;
    output_params->pa_cfg.pa_hp_sel     = pa_pwr_cfg->pa_config.pa_hp_sel;
    output_params->pa_cfg.pa_reg_supply = pa_pwr_cfg->pa_config.pa_reg_supply;
    output_params->pa_cfg.pa_sel        = pa_pwr_cfg->pa_config.pa_sel;

    output_params->pa_ramp_time = LR11XX_RADIO_RAMP_48_US;
}

void ral_lr11xx_bsp_get_reg_mode( const void* context, lr11xx_system_reg_mode_t* reg_mode )
{
    *reg_mode = smtc_shield_lr11xx_get_reg_mode( );
}

void ral_lr11xx_bsp_get_xosc_cfg( const void* context, bool* tcxo_is_radio_controlled,
                                  lr11xx_system_tcxo_supply_voltage_t* supply_voltage, uint32_t* startup_time_in_tick )
{
    const smtc_shield_lr11xx_tcxo_cfg_t tcxo_cfg = smtc_shield_lr11xx_get_tcxo_cfg( );

    *tcxo_is_radio_controlled = tcxo_cfg.has_tcxo;
    *supply_voltage           = tcxo_cfg.supply;
    *startup_time_in_tick     = tcxo_cfg.startup_time_in_tick;
}

void ral_lr11xx_bsp_get_crc_state( const void* context, bool* crc_is_activated )
{
#if defined( USE_LR11XX_CRC_OVER_SPI )
    SMTC_HAL_TRACE_INFO( "LR11XX CRC over spi is activated\n" );
    *crc_is_activated = true;
#else
    *crc_is_activated = false;
#endif
}

void ral_lr11xx_bsp_get_rssi_calibration_table( const void* context, const uint32_t freq_in_hz,
                                                lr11xx_radio_rssi_calibration_table_t* rssi_calibration_table )
{
    if( freq_in_hz <= 600000000 )
    {
        rssi_calibration_table->gain_offset      = 0;
        rssi_calibration_table->gain_tune.g11    = 12;
        rssi_calibration_table->gain_tune.g10    = 12;
        rssi_calibration_table->gain_tune.g9     = 14;
        rssi_calibration_table->gain_tune.g8     = 0;
        rssi_calibration_table->gain_tune.g7     = 3;
        rssi_calibration_table->gain_tune.g6     = 1;
        rssi_calibration_table->gain_tune.g5     = 4;
        rssi_calibration_table->gain_tune.g4     = 4;
        rssi_calibration_table->gain_tune.g13hp6 = 6;
        rssi_calibration_table->gain_tune.g13hp5 = 3;
        rssi_calibration_table->gain_tune.g13hp4 = 6;
        rssi_calibration_table->gain_tune.g13hp3 = 6;
        rssi_calibration_table->gain_tune.g13hp2 = 6;
        rssi_calibration_table->gain_tune.g13hp1 = 6;
        rssi_calibration_table->gain_tune.g13    = 6;
        rssi_calibration_table->gain_tune.g12    = 6;
        rssi_calibration_table->gain_tune.g13hp7 = 6;
    }
    else if( 2000000000 <= freq_in_hz )
    {
        rssi_calibration_table->gain_offset      = 2018;
        rssi_calibration_table->gain_tune.g11    = 7;
        rssi_calibration_table->gain_tune.g10    = 6;
        rssi_calibration_table->gain_tune.g9     = 4;
        rssi_calibration_table->gain_tune.g8     = 6;
        rssi_calibration_table->gain_tune.g7     = 4;
        rssi_calibration_table->gain_tune.g6     = 3;
        rssi_calibration_table->gain_tune.g5     = 12;
        rssi_calibration_table->gain_tune.g4     = 14;
        rssi_calibration_table->gain_tune.g13hp6 = 12;
        rssi_calibration_table->gain_tune.g13hp5 = 14;
        rssi_calibration_table->gain_tune.g13hp4 = 12;
        rssi_calibration_table->gain_tune.g13hp3 = 12;
        rssi_calibration_table->gain_tune.g13hp2 = 8;
        rssi_calibration_table->gain_tune.g13hp1 = 12;
        rssi_calibration_table->gain_tune.g13    = 9;
        rssi_calibration_table->gain_tune.g12    = 8;
        rssi_calibration_table->gain_tune.g13hp7 = 9;
    }
    else
    {
        rssi_calibration_table->gain_offset      = 0;
        rssi_calibration_table->gain_tune.g11    = 2;
        rssi_calibration_table->gain_tune.g10    = 2;
        rssi_calibration_table->gain_tune.g9     = 3;
        rssi_calibration_table->gain_tune.g8     = 2;
        rssi_calibration_table->gain_tune.g7     = 4;
        rssi_calibration_table->gain_tune.g6     = 3;
        rssi_calibration_table->gain_tune.g5     = 4;
        rssi_calibration_table->gain_tune.g4     = 5;
        rssi_calibration_table->gain_tune.g13hp6 = 6;
        rssi_calibration_table->gain_tune.g13hp5 = 4;
        rssi_calibration_table->gain_tune.g13hp4 = 5;
        rssi_calibration_table->gain_tune.g13hp3 = 5;
        rssi_calibration_table->gain_tune.g13hp2 = 6;
        rssi_calibration_table->gain_tune.g13hp1 = 6;
        rssi_calibration_table->gain_tune.g13    = 7;
        rssi_calibration_table->gain_tune.g12    = 8;
        rssi_calibration_table->gain_tune.g13hp7 = 6;
    }
}

/* --- EOF ------------------------------------------------------------------ */
