#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_ATCOMMANDS 0xffff0001

// Basic syscall wrappers.

int at_commands_set_tx_callback(subscribe_cb callback, void* callback_args);
int at_commands_set_rx_callback(subscribe_cb callback, void* callback_args);
int at_commands_set_tx_buffer(uint8_t* buffer, uint32_t len);
int at_commands_set_rx_buffer(uint8_t* buffer, uint32_t len);

// Tell the underlying hardware to transmit the AT command stored in the TX
// allowed buffer to the radio. The length provided is the length of the AT
// command to send.
int at_commands_send_tx_buffer(uint32_t length);

// Tell the underlying hardware to begin listening for a message from the radio.
// This can be called at any time.
int at_commands_wait_for_rx(void);

// Send an AT command and wait for the response.
//
// Arguments:
// - tx_buffer:                 Pointer to a buffer that contains the AT command
//                              to send.
// - tx_buffer_length:          Number of bytes in that tx buffer.
// - tx_buffer_transmit_length: Number of bytes in the actual AT command.
// - rx_buffer:                 Pointer to a buffer that the response should be
//                              copied into.
// - rx_buffer_length:          Number of bytes in that rx buffer.
int at_commands_txrx_sync(uint8_t* tx_buffer,
                          int tx_buffer_length,
                          int tx_buffer_transmit_length,
                          uint8_t* rx_buffer,
                          int rx_buffer_length);

// Setup a receive buffer and wait for any UART incoming message.
int at_commands_rx_sync(uint8_t* rx_buffer, int rx_buffer_length);



#ifdef __cplusplus
}
#endif
