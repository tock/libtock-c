
#include <string.h>
#include <stdio.h>

#include "smtc_hal_spi.h"
#include "smtc_hal_config.h"

#include <libtock/peripherals/spi_controller.h>
#include <libtock-sync/net/lora_phy.h>


static uint8_t m_tx_buf[256] = { 0 };
static uint8_t m_rx_buf[256] = { 0 };

static bool spi_initialized = false;

void hal_spi_init( void )
{
    if( spi_initialized == false )
    {
        spi_initialized = true;
        // Set the SPI clock rate to 4MHz (NRF_SPIM_FREQ_4M)
        libtock_spi_controller_set_rate(4000000); // 4MHz as an integer
        // Set the SPI phase to sample on a leading (low to high) clock edge
        libtock_spi_controller_set_phase(false);
        // Set the SPI polarity so that the idle clock is low
        libtock_spi_controller_set_polarity(false);
    }
}

void hal_spi_deinit( void )
{
    if( spi_initialized == true )
    {
        spi_initialized = false;
    }
}

void hal_spi_write( const uint8_t* buffer, uint16_t length )
{
    if( spi_initialized == true )
    {
        libtocksync_lora_phy_write((const char*)buffer, (size_t)length);
    }
}

// we don't have read only function implemented in Tock
void hal_spi_read( uint8_t* buffer, uint16_t length )
{
    if( spi_initialized == true )
    {
    }
}

void hal_spi_write_read( const uint8_t* cbuffer, uint8_t* rbuffer, uint16_t length )
{
    if( spi_initialized == true )
    {
        libtocksync_lora_phy_read_write((const char*)cbuffer, (char*)rbuffer, (size_t)length);
    }
}

void hal_spi_read_with_dummy_byte( uint8_t* buffer, uint16_t length, uint8_t dummy_byte )
{
    if( spi_initialized == true )
    {
        if( length <= sizeof( m_tx_buf ))
        {
            memset( m_tx_buf, dummy_byte, length );
            libtocksync_lora_phy_read_write((const char*)m_tx_buf, (char*)buffer, (size_t)length);
        }
        else
        {
            printf( "error: spi m_tx_buf is emty\r\n" );
        }
    }
}

uint16_t hal_spi_in_out( const uint32_t id, const uint16_t out_data )
{
	uint8_t tv = 0, rv = 0;
    if( spi_initialized == true )
    {
        tv = ( uint8_t )( out_data & 0xFF );
        libtocksync_lora_phy_read_write((const char*)&tv, (char*)&rv, 1);
    }
	return rv;
}
