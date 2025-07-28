#pragma once

#include <libtock/peripherals/spi_controller.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

bool libtocksync_spi_controller_exists(void);

// As the SPI, write a buffer of data.
returncode_t libtocksync_spi_controller_write(const uint8_t* write,
                                              size_t         len);

// As the SPI, write and read buffers of data.
returncode_t libtocksync_spi_controller_read_write(const uint8_t* write,
                                                   uint8_t*       read,
                                                   size_t         len);

#ifdef __cplusplus
}
#endif
