#pragma once

#include "../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

// Check if the driver exists.
bool libtock_nrf51_serialization_exists(void);

// Toggle the reset line to the nRF51 chip to reset the BLE MCU.
returncode_t libtock_nrf51_serialization_reset(void);

// Write a packet to the BLE Serialization connectivity processor.
returncode_t libtock_nrf51_serialization_write(uint8_t* tx, int tx_len);

// Receive into the buffer passed in to
// `libtock_nrf51_serialization_set_readwrite_allow_receive_buffer` previously.
returncode_t libtock_nrf51_serialization_read(int rx_len, int* bytes_read);

#ifdef __cplusplus
}
#endif
