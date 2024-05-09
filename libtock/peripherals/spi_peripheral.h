#pragma once

#include "../tock.h"
#include "syscalls/spi_peripheral_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for SPI peripheral callback.
//
// - `arg1` (`returncode_t`): Status from reading/writing SPI data.
typedef void (*libtock_spi_peripheral_callback)(returncode_t);


// Get the chip select. This will always return 0.
returncode_t libtock_spi_peripheral_get_chip_select(uint32_t* chip_select);

// Get the SPI phase.
returncode_t libtock_spi_peripheral_get_phase(uint32_t* phase);

// Set the SPI phase.
returncode_t libtock_spi_peripheral_set_phase(bool phase);

// Get the SPI polarity.
returncode_t libtock_spi_peripheral_get_polarity(uint32_t* polarity);

// Set the SPI polarity.
returncode_t libtock_spi_peripheral_set_polarity(bool polarity);

// Write a buffer on the SPI bus. The callback will be triggered after it is
// written.
returncode_t libtock_spi_peripheral_write(const uint8_t*                         buffer,
                                          size_t                          len,
                                          libtock_spi_peripheral_callback cb);

// Write a buffer on the SPI bus and also capture the incoming data. The
// callback will be triggered after it is written.
returncode_t libtock_spi_peripheral_read_write(const uint8_t*                  write,
                                               uint8_t*                        read,
                                               size_t                          len,
                                               libtock_spi_peripheral_callback cb);

#ifdef __cplusplus
}
#endif
