/*!
 * @file      smtc_shield_lr1110mb1dxs.c
 *
 * @brief     LR1110MB1DxS-specific function implementations
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

#include <stddef.h>
#include "smtc_shield_lr11xx_common_if.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

#define LR1110MB1DXS_SUBGHZ_FREQ_MIN 150000000
#define LR1110MB1DXS_SUBGHZ_FREQ_MAX 960000000

#define LR1110MB1DXS_MIN_PWR -17
#define LR1110MB1DXS_MAX_PWR 22

#define LR1110MB1DXS_MIN_PWR_PA_HF -18
#define LR1110MB1DXS_MAX_PWR_PA_HF 13

// PA config table
const smtc_shield_lr11xx_pa_pwr_cfg_t pa_cfg_table[LR1110MB1DXS_MAX_PWR - LR1110MB1DXS_MIN_PWR + 1] = {
{ // Expected output power = -17dBm
        .power = -15,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = -16dBm
        .power = -14,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = -15dBm
        .power = -13,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = -14dBm
        .power = -12,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = -13dBm
        .power = -11,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = -12dBm
        .power = -9,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = -11dBm
        .power = -8,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = -10dBm
        .power = -7,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = -9dBm
        .power = -6,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = -8dBm
        .power = -5,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = -7dBm
        .power = -4,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = -6dBm
        .power = -3,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = -5dBm
        .power = -2,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = -4dBm
        .power = -1,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = -3dBm
        .power = 0,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = -2dBm
        .power = 1,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = -1dBm
        .power = 2,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = 0dBm
        .power = 3,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = 1dBm
        .power = 3,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x01,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = 2dBm
        .power = 4,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x01,
            .pa_hp_sel     = 0x00,
        },
    },
    {  // Expected output power = 3dBm
        .power = 7,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    {  // Expected output power = 4dBm
        .power = 8,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    {  // Expected output power = 5dBm
        .power = 9, 
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    {  // Expected output power = 6dBm
        .power = 10,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    {  // Expected output power = 7dBm
        .power = 12,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = 8dBm
        .power = 13,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = 9dBm
        .power = 14,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x00,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = 10dBm
        .power = 13,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x01,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = 11dBm
        .power = 13,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x02,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = 12dBm
        .power = 14,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x02,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = 13dBm
        .power = 14,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x03,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = 14dBm
        .power = 14,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x04,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = 15dBm
        .power = 14,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 0x07,
            .pa_hp_sel     = 0x00,
        },
    },
    { // Expected output power = 16dBm
        .power = 22,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_HP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VBAT,
            .pa_duty_cycle = 0x01,
            .pa_hp_sel     = 0x04,
        },
    },
    { // Expected output power = 17dBm
        .power = 22,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_HP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VBAT,
            .pa_duty_cycle = 0x02,
            .pa_hp_sel     = 0x04,
        },
    },
    { // Expected output power = 18dBm
        .power = 22,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_HP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VBAT,
            .pa_duty_cycle = 0x01,
            .pa_hp_sel     = 0x06,
        },
    },
    { // Expected output power = 19dBm
        .power = 22,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_HP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VBAT,
            .pa_duty_cycle = 0x03,
            .pa_hp_sel     = 0x05,
        },
    },
    { // Expected output power = 20dBm
        .power = 22,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_HP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VBAT,
            .pa_duty_cycle = 0x04,
            .pa_hp_sel     = 0x07,
        },
    },
    { // Expected output power = 21dBm
        .power = 22,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_HP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VBAT,
            .pa_duty_cycle = 0x05,
            .pa_hp_sel     = 0x07,
        },
    },
    { // Expected output power = 22dBm
        .power = 22,
        .pa_config = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_HP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VBAT,
            .pa_duty_cycle = 0x06,
            .pa_hp_sel     = 0x07,
        },
    },
};

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
 * --- PUBLIC VARIABLES --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

const smtc_shield_lr11xx_pa_pwr_cfg_t* smtc_shield_lr11xx_get_pa_pwr_cfg( const uint32_t rf_freq_in_hz,
                                                                          int8_t         expected_output_pwr_in_dbm )
{
    if( ( LR1110MB1DXS_SUBGHZ_FREQ_MIN <= rf_freq_in_hz ) && ( rf_freq_in_hz <= LR1110MB1DXS_SUBGHZ_FREQ_MAX ) )
    {
        if( ( LR1110MB1DXS_MIN_PWR <= expected_output_pwr_in_dbm ) &&
            ( expected_output_pwr_in_dbm <= LR1110MB1DXS_MAX_PWR ) )
        {
            return &( pa_cfg_table[expected_output_pwr_in_dbm - LR1110MB1DXS_MIN_PWR] );
        }
    }

    return NULL;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/* --- EOF ------------------------------------------------------------------ */
