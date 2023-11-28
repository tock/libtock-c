
#ifndef _SMTC_HAL_SPI_H
#define _SMTC_HAL_SPI_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void hal_spi_init( void );

void hal_spi_deinit( void );

void hal_spi_write( const uint8_t* buffer, uint16_t length );

void hal_spi_read( uint8_t* buffer, uint16_t length );

void hal_spi_write_read( const uint8_t* cbuffer, uint8_t* rbuffer, uint16_t length );

void hal_spi_read_with_dummy_byte( uint8_t* buffer, uint16_t length, uint8_t dummy_byte );

uint16_t hal_spi_in_out( const uint32_t id, const uint16_t out_data );

#ifdef __cplusplus
}
#endif

#endif
