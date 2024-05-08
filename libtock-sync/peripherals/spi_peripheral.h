#pragma once

#include <libtock/peripherals/spi_peripheral.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// As the SPI peripheral, write a buffer of data.
returncode_t libtocksync_spi_peripheral_write(const uint8_t* write,
                                              size_t         len);

// As the SPI peripheral, write and read buffers of data.
returncode_t libtocksync_spi_peripheral_read_write(const uint8_t* write,
                                                   uint8_t*       read,
                                                   size_t         len);

#ifdef __cplusplus
}
#endif
