#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_SPI_PERIPHERAL 0x20002

// Check if the SPI peripheral driver is available on this board.
bool libtock_spi_peripheral_exists(void);

// Set the upcall function.
returncode_t libtock_spi_peripheral_set_upcall(subscribe_upcall callback, void* opaque);

// Allow a buffer for reading SPI data.
returncode_t libtock_spi_peripheral_allow_readwrite_read(uint8_t* buffer, uint32_t len);

// Allow a buffer for writing SPI data.
returncode_t libtock_spi_peripheral_allow_readonly_write(uint8_t* buffer, uint32_t len);

// Command for SPI write.
returncode_t libtock_spi_peripheral_command_write(uint32_t len);

// Return the chip select. This will always return 0.
returncode_t libtock_spi_peripheral_command_get_chip_select(uint32_t* chip_select);

// Get the current SPI phase.
returncode_t libtock_spi_peripheral_command_get_phase(uint32_t* phase);

// Set the SPI phase.
returncode_t libtock_spi_peripheral_command_set_phase(bool phase);

// Get the current SPI polarity.
returncode_t libtock_spi_peripheral_command_get_polarity(uint32_t* polarity);

// Set the SPI polarity.
returncode_t libtock_spi_peripheral_command_set_polarity(bool polarity);

#ifdef __cplusplus
}
#endif