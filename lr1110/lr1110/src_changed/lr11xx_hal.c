/*!
 * \file      lr11xx_hal.c
 *
 * \brief     Implements the lr11xx radio HAL functions
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
#include <string.h>   // memset

#include <libtock-sync/services/alarm.h>
#include <libtock-sync/net/lora_phy.h>

#include "lr11xx_hal.h"
#include "smtc_hal_gpio.h"
#include "smtc_hal_spi.h"
//#include "smtc_hal_mcu.h"

#include "lr11xx_hal_context.h"
#include "lr11xx_system_types.h"
#include "lr11xx_system.h"

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

typedef enum
{
    RADIO_SLEEP,
    RADIO_AWAKE
} radio_mode_t;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

static volatile radio_mode_t radio_mode = RADIO_AWAKE;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/**
 * @brief Wait until radio busy pin returns to 0
 */
static void lr11xx_hal_wait_on_busy( const uint32_t busy_pin );

/**
 * @brief Check if device is ready to receive spi transaction.
 * @remark If the device is in sleep mode, it will awake it and wait until it is ready
 */
static void lr11xx_hal_check_device_ready( const lr11xx_hal_context_t* lr11xx_context );

/**
 * @brief Disables interruptions used in Modem (radio_dio and timer)
 */
static void modem_disable_irq( void );

/**
 * @brief Enables interruptions used in Modem (radio_dio and timer)
 */
static void modem_enable_irq( void );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

lr11xx_hal_status_t lr11xx_hal_write( const void* context, const uint8_t* command, const uint16_t command_length,
                                      const uint8_t* data, const uint16_t data_length )
{
#if defined( USE_LR11XX_CRC_OVER_SPI )
    // Compute the CRC over command array first and over data array then
    uint8_t cmd_crc = lr11xx_hal_compute_crc( 0xFF, command, command_length );
    cmd_crc         = lr11xx_hal_compute_crc( cmd_crc, data, data_length );
#endif

    const lr11xx_hal_context_t* lr11xx_context = ( const lr11xx_hal_context_t* ) context;

    lr11xx_hal_check_device_ready( lr11xx_context );

    // Disable IRQ to secure LR11XX concurrent access
    modem_disable_irq( );

    // Create one wbuffer we can send to the SPI interface via the kernel.
    uint8_t rbuffer[100];
    uint8_t wbuffer[100];

    int write_length = command_length + data_length;

    // Pack the entire message in one wbuffer.
    memcpy(wbuffer, command, command_length);
    memcpy(wbuffer+command_length, data, data_length);

#if defined( USE_LR11XX_CRC_OVER_SPI )
    // Send the CRC byte at the end of the transaction
    wbuffer[write_length] = cmd_crc;
    write_length += 1;
#endif

    // Do the SPI transfer.
    libtocksync_lora_phy_read_write((const char*)wbuffer, (const char*)rbuffer,  write_length);

    // LR11XX_SYSTEM_SET_SLEEP_OC=0x011B opcode. In sleep mode the radio busy line is held at 1 => do not test it
    if( ( command[0] == 0x01 ) && ( command[1] == 0x1B ) )
    {
        radio_mode = RADIO_SLEEP;

        // add a incompressible delay to prevent trying to wake the radio before it is full asleep
        libtocksync_alarm_delay_ms( 1 );

        //
        hal_spi_deinit( );
    }

    // Re-enable IRQ
    modem_enable_irq( );

    return LR11XX_HAL_STATUS_OK;
}

lr11xx_hal_status_t lr11xx_hal_read( const void* context, const uint8_t* command, const uint16_t command_length,
                                     uint8_t* data, const uint16_t data_length )
{
    const lr11xx_hal_context_t* lr11xx_context = ( const lr11xx_hal_context_t* ) context;

    lr11xx_hal_check_device_ready( lr11xx_context );

    // Input buffer.
    uint8_t rbuffer[100];
    uint8_t wbuffer[100];

    memcpy(wbuffer, command, command_length);

    // Do the SPI transaction to request data.
    libtocksync_lora_phy_read_write((const char*)wbuffer, (char*)rbuffer, command_length);

    lr11xx_hal_check_device_ready( lr11xx_context );

    memset(wbuffer, 0, sizeof(wbuffer));

    // Do the SPI transaction to read data.
    libtocksync_lora_phy_read_write((const char*)wbuffer, (char*)rbuffer, data_length+1);

    memcpy(data, rbuffer+1, data_length);

    return LR11XX_HAL_STATUS_OK;
}

lr11xx_hal_status_t lr11xx_hal_direct_read( const void* radio, uint8_t* data, const uint16_t data_length )
{
    const lr11xx_hal_context_t* lr11xx_context = ( const lr11xx_hal_context_t* ) radio;

    lr11xx_hal_check_device_ready( lr11xx_context );

    uint8_t wbuffer[100];

    memset(wbuffer, 0, sizeof(wbuffer));

    // Write all zeros on SPI and read in the incoming data.
    libtocksync_lora_phy_read_write(wbuffer, data, data_length);

    return LR11XX_HAL_STATUS_OK;
}

lr11xx_hal_status_t lr11xx_hal_reset( const void* context )
{
    const lr11xx_hal_context_t* lr11xx_context = ( const lr11xx_hal_context_t* ) context;
    hal_gpio_set_value( lr11xx_context->reset, 0 );
    libtocksync_alarm_delay_ms( 5 );
    hal_gpio_set_value( lr11xx_context->reset, 1 );
    libtocksync_alarm_delay_ms( 5 );

    // Wait until internal lr11xx fw is ready
    libtocksync_alarm_delay_ms( 250 );

    radio_mode = RADIO_AWAKE;

    return LR11XX_HAL_STATUS_OK;
}

lr11xx_hal_status_t lr11xx_hal_wakeup( const void* context )
{
    const lr11xx_hal_context_t* lr11xx_context = ( const lr11xx_hal_context_t* ) context;
    lr11xx_hal_check_device_ready( lr11xx_context );
    return LR11XX_HAL_STATUS_OK;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

static void lr11xx_hal_wait_on_busy( const uint32_t busy_pin )
{
    uint32_t cnt = 0;
    while( hal_gpio_get_value( busy_pin ) == 1 )
    {
        libtocksync_alarm_delay_ms( 1 );
        cnt ++;
        if( cnt >= 3000 )
        {
            // printf( "lr1110_hal_wait_on_busy\r\n" );
            break;
        }
    };
}

static void lr11xx_hal_check_device_ready( const lr11xx_hal_context_t* lr11xx_context )
{
    if( radio_mode != RADIO_SLEEP )
    {
        lr11xx_hal_wait_on_busy( lr11xx_context->busy );
    }
    else
    {
        hal_spi_init( );

        // Busy is HIGH in sleep mode, wake-up the device with a small glitch on NSS
        hal_gpio_set_value( lr11xx_context->nss, 0 );
        hal_gpio_set_value( lr11xx_context->nss, 1 );
        lr11xx_hal_wait_on_busy( lr11xx_context->busy );
        radio_mode = RADIO_AWAKE;
    }
}

static void modem_disable_irq( void )
{
    hal_gpio_irq_disable( );
}

static void modem_enable_irq( void )
{
    hal_gpio_irq_enable( );
}

/* --- EOF ------------------------------------------------------------------ */
