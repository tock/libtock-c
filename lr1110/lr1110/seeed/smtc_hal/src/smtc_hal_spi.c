
#include "nrfx_spim.h"
#include "smtc_hal_spi.h"
#include "smtc_hal_config.h"

static const nrfx_spim_t spi = NRFX_SPIM_INSTANCE( 3 );
static uint8_t m_tx_buf[256] = { 0 };
static uint8_t m_rx_buf[256] = { 0 };

static bool spi_init = false;

void hal_spi_init( void )
{
    if( spi_init == false )
    {
        spi_init = true;
        nrfx_spim_config_t spi_config = NRFX_SPIM_DEFAULT_CONFIG;
        spi_config.frequency = NRF_SPIM_FREQ_4M;
        spi_config.mode = NRF_SPIM_MODE_0;
        spi_config.bit_order = NRF_SPIM_BIT_ORDER_MSB_FIRST;
        spi_config.miso_pin = LR1110_SPI_MISO_PIN;
        spi_config.mosi_pin = LR1110_SPI_MOSI_PIN;
        spi_config.sck_pin = LR1110_SPI_SCK_PIN;
        // spi_config.ss_pin = LR1110_SPI_NSS_PIN;
        // spi_config.use_hw_ss = true;
        spi_config.use_hw_ss = false;
        spi_config.ss_active_high = false;
        nrfx_spim_init( &spi, &spi_config, NULL, NULL );
    }
}

void hal_spi_deinit( void )
{
    if( spi_init == true )
    {
        spi_init = false;
        nrfx_spim_uninit( &spi );
    }
}

void hal_spi_write( const uint8_t* buffer, uint16_t length )
{
    if( spi_init == true )
    {
        nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TX( buffer, length );
        nrfx_spim_xfer( &spi, &xfer_desc, NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER );
    }
}

void hal_spi_read( uint8_t* buffer, uint16_t length )
{
    if( spi_init == true )
    {
        system_spi_write_read( buffer, buffer, length );
    }
}

void hal_spi_write_read( const uint8_t* cbuffer, uint8_t* rbuffer, uint16_t length )
{
    if( spi_init == true )
    {
        nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TRX( cbuffer, length, rbuffer, length );
        nrfx_spim_xfer( &spi, &xfer_desc, NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER );
    }
}

void hal_spi_read_with_dummy_byte( uint8_t* buffer, uint16_t length, uint8_t dummy_byte )
{
    if( spi_init == true )
    {
        if( length <= sizeof( m_tx_buf ))
        { 
            memset( m_tx_buf, dummy_byte, length );
            nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TRX( m_tx_buf, length, buffer, length );
            nrfx_spim_xfer( &spi, &xfer_desc, NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER );
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
    if( spi_init == true )
    {
        tv = ( uint8_t )( out_data & 0xFF );
        nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TRX(( uint8_t *)(&tv), 1, ( uint8_t *)(&rv), 1 );
        nrfx_spim_xfer( &spi, &xfer_desc, NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER );
    }
	return rv;
}
