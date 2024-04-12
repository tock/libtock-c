#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_SPI_CONTROLLER 0x20001

// Check if the SPI driver is available on this board.
bool libtock_spi_controller_exists(void);

// Set the upcall function.
returncode_t libtock_spi_controller_set_upcall(subscribe_upcall callback, void* opaque);

// Allow a buffer for reading SPI data.
returncode_t libtock_spi_controller_allow_readwrite_read(uint8_t* buffer, uint32_t len);

// Allow a buffer for writing SPI data.
returncode_t libtock_spi_controller_allow_readonly_write(uint8_t* buffer, uint32_t len);

// Return the chip select. This will always return 0.
returncode_t libtock_spi_controller_command_get_chip_select(uint32_t* chip_select);

// Set the chip select. Not supported in upstream kernel as of 4/9/24.
returncode_t libtock_spi_controller_command_set_chip_select(uint32_t chip_select);

// Set the SPI rate.
returncode_t libtock_spi_controller_command_set_rate(uint32_t rate);

// Get the SPI rate.
returncode_t libtock_spi_controller_command_get_rate(uint32_t* rate);

// Get the current SPI phase.
returncode_t libtock_spi_controller_command_get_phase(uint32_t* phase);

// Set the SPI phase.
returncode_t libtock_spi_controller_command_set_phase(bool phase);

// Get the current SPI polarity.
returncode_t libtock_spi_controller_command_get_polarity(uint32_t* polarity);

// Set the SPI polarity.
returncode_t libtock_spi_controller_command_set_polarity(bool polarity);

// Hold chip select line low over multiple SPI operations.
// Only partially supported depending on implementation.
returncode_t libtock_spi_controller_command_hold_low(void);

// Release chip select line low.
returncode_t libtock_spi_controller_command_release_low(void);

// Write a single byte
returncode_t libtock_spi_controller_command_write_byte(uint32_t byte);

// Read / Write into allowed buffers
returncode_t libtock_spi_controller_command_read_write_bytes(uint32_t len);

#ifdef __cplusplus
}
#endif
