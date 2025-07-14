#pragma once

#include "../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for SPI callback.
//
// - `arg1` (`returncode_t`): Status from reading/writing SPI data.
typedef void (*libtock_spi_controller_callback)(returncode_t);

// Check if the driver exists.
bool libtock_spi_controller_exists(void);

// Set the chip select.
returncode_t libtock_spi_controller_set_chip_select(uint32_t chip_select);

// Get the chip select. This will always return 0.
returncode_t libtock_spi_controller_get_chip_select(uint32_t* chip_select);

/* Set the rate. Rate is the Hz of the SPI clock. So a rate of 100000
 * is a 100kHZ clock. */
returncode_t libtock_spi_controller_set_rate(uint32_t rate);

/* Get the rate. Rate is the Hz of the SPI clock. So a rate of 100000
 * is a 100kHZ clock. */
returncode_t libtock_spi_controller_get_rate(uint32_t* rate);

/* Set the SPI phase.
 * false means sample on a leading (low to high) clock edge
 * true means sample on a trailing (high to low) clock edge */
returncode_t libtock_spi_controller_set_phase(bool phase);

// Get the SPI phase.
returncode_t libtock_spi_controller_get_phase(uint32_t* phase);

// Set the SPI polarity. False means idle clock is low.
returncode_t libtock_spi_controller_set_polarity(bool polarity);

// Get the SPI polarity.
returncode_t libtock_spi_controller_get_polarity(uint32_t* polarity);

/* Only partially supported, depending on implementation. In some cases
   allows a process to hold its chip select line low over multiple SPI
   operations*/
returncode_t libtock_spi_controller_hold_low(void);

// Releases a held low chip select line, if supported.
returncode_t libtock_spi_controller_release_low(void);

returncode_t libtock_spi_controller_write_byte(unsigned char byte);

returncode_t libtock_spi_controller_write(const uint8_t*                  buffer,
                                          size_t                          len,
                                          libtock_spi_controller_callback cb);

returncode_t libtock_spi_controller_read_write(const uint8_t*                  write,
                                               uint8_t*                        read,
                                               size_t                          len,
                                               libtock_spi_controller_callback cb);

#ifdef __cplusplus
}
#endif
