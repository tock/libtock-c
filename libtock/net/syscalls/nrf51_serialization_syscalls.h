#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_NRF51_SERIALIZATION 0x80004

// Check if this driver exists.
bool libtock_nrf51_serialization_exists(void);

// Set the upcall function.
returncode_t libtock_nrf51_serialization_set_upcall(subscribe_upcall callback, void* opaque);

// Pass the RX buffer for the UART module to use.
returncode_t libtock_nrf51_serialization_set_readwrite_allow_receive_buffer(uint8_t* rx, uint32_t rx_len);

// Set the buffer to send from.
returncode_t libtock_nrf51_serialization_set_readonly_allow_write_buffer(uint8_t* tx, uint32_t tx_len);

// Reset the nRF51 chip
returncode_t libtock_nrf51_serialization_command_reset(void);

// Write the write buffer.
returncode_t libtock_nrf51_serialization_command_write(void);

// Read into the read buffer. `bytes_read` is set to the number of bytes
// received.
returncode_t libtock_nrf51_serialization_command_read(uint32_t rx_len, uint32_t* bytes_read);

#ifdef __cplusplus
}
#endif

