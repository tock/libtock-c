
// #include "nrfx_spim.h"
#include "smtc_hal_spi.h"
#include "smtc_hal_config.h"
#include <libtock/peripherals/spi_controller.h>
#include <libtock-sync/net/lora_phy.h>
#include <string.h>
#include <stdio.h>

// static const nrfx_spim_t spi = NRFX_SPIM_INSTANCE( 3 );
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
        // Set the chip select pin
        // spi_set_chip_select(LR1110_SPI_NSS_PIN);

        // spi_init = true;
        // nrfx_spim_config_t spi_config = NRFX_SPIM_DEFAULT_CONFIG;
        // spi_config.frequency = NRF_SPIM_FREQ_4M;
        // spi_config.mode = NRF_SPIM_MODE_0;
        // spi_config.bit_order = NRF_SPIM_BIT_ORDER_MSB_FIRST;
        // spi_config.miso_pin = LR1110_SPI_MISO_PIN;
        // spi_config.mosi_pin = LR1110_SPI_MOSI_PIN;
        // spi_config.sck_pin = LR1110_SPI_SCK_PIN;
        // // spi_config.ss_pin = LR1110_SPI_NSS_PIN;
        // // spi_config.use_hw_ss = true;
        // spi_config.use_hw_ss = false;
        // spi_config.ss_active_high = false;
        // nrfx_spim_init( &spi, &spi_config, NULL, NULL );
    }
}

void hal_spi_deinit( void )
{
    if( spi_initialized == true )
    {
        spi_initialized = false;
        // nrfx_spim_uninit( &spi );
    }
}

void hal_spi_write( const uint8_t* buffer, uint16_t length )
{
    if( spi_initialized == true )
    {
        libtocksync_lora_phy_write((const char*)buffer, (size_t)length);

        // nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TX( buffer, length );
        // nrfx_spim_xfer( &spi, &xfer_desc, NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER );
    }
}

// we don't have read only function implemented in Tock
void hal_spi_read( uint8_t* buffer, uint16_t length )
{
    if( spi_initialized == true )
    {
        // system_spi_write_read( buffer, buffer, length );
    }
}

void hal_spi_write_read( const uint8_t* cbuffer, uint8_t* rbuffer, uint16_t length )
{
    if( spi_initialized == true )
    {
        libtocksync_lora_phy_read_write((const char*)cbuffer, (char*)rbuffer, (size_t)length);


        // nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TRX( cbuffer, length, rbuffer, length );
        // nrfx_spim_xfer( &spi, &xfer_desc, NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER );
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
            // nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TRX( m_tx_buf, length, buffer, length );
            // nrfx_spim_xfer( &spi, &xfer_desc, NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER );
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

        // nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TRX(( uint8_t *)(&tv), 1, ( uint8_t *)(&rv), 1 );
        // nrfx_spim_xfer( &spi, &xfer_desc, NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER );
    }
	return rv;
}
