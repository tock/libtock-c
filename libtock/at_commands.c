#include "at_commands.h"
#include "tock.h"

struct at_commands_data {
  bool fired;
  int length;
};

// Internal callback for faking synchronous reads
static void at_commands_cb(__attribute__ ((unused)) int error_code,
                           int length,
                           __attribute__ ((unused)) int unused,
                           void* ud) {
  struct at_commands_data* data = (struct at_commands_data*) ud;
  data->length = length;
  data->fired  = true;
}


int at_commands_set_tx_callback(subscribe_cb callback, void* callback_args) {
  return subscribe(DRIVER_NUM_ATCOMMANDS, 1, callback, callback_args);
}

int at_commands_set_rx_callback(subscribe_cb callback, void* callback_args) {
  return subscribe(DRIVER_NUM_ATCOMMANDS, 2, callback, callback_args);
}

int at_commands_set_tx_buffer(uint8_t* buffer, uint32_t len) {
  return allow(DRIVER_NUM_ATCOMMANDS, 1, (void*) buffer, len);
}

int at_commands_set_rx_buffer(uint8_t* buffer, uint32_t len) {
  return allow(DRIVER_NUM_ATCOMMANDS, 2, (void*) buffer, len);
}

int at_commands_send_tx_buffer(uint32_t length) {
  return command(DRIVER_NUM_ATCOMMANDS, 1, length, 0);
}

int at_commands_wait_for_rx(void) {
  return command(DRIVER_NUM_ATCOMMANDS, 2, 0, 0);
}


int at_commands_txrx_sync(uint8_t* tx_buffer,
                          int tx_buffer_length,
                          int tx_buffer_transmit_length,
                          uint8_t* rx_buffer,
                          int rx_buffer_length) {
  int err;
  struct at_commands_data result = {.fired = false};

  // The callback we care about is the RX callback, so wait for that.
  err = at_commands_set_rx_callback(at_commands_cb, (void*) &result);
  if (err < 0) return err;

  // Setup the two buffers that were passed to this function.
  err = at_commands_set_tx_buffer(tx_buffer, tx_buffer_length);
  if (err < 0) return err;
  err = at_commands_set_rx_buffer(rx_buffer, rx_buffer_length);
  if (err < 0) return err;

  // Make sure that the UART hardware is listening for incoming messages.
  err = at_commands_wait_for_rx();
  if (err < 0) return err;

  // Now tell the UART driver to TX. This will transmit the needed bytes and
  // then hopefully the chip sends a response. We will get a callback after the
  // RX happens.
  err = at_commands_send_tx_buffer(tx_buffer_transmit_length);
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  return result.length;
}

int at_commands_rx_sync(uint8_t* rx_buffer,
                        int rx_buffer_length) {
  int err;
  struct at_commands_data result = {.fired = false};

  // The callback we care about is the RX callback, so wait for that.
  err = at_commands_set_rx_callback(at_commands_cb, (void*) &result);
  if (err < 0) return err;

  // Setup RX buffer.
  err = at_commands_set_rx_buffer(rx_buffer, rx_buffer_length);
  if (err < 0) return err;

  // Make sure that the UART hardware is listening for incoming messages.
  err = at_commands_wait_for_rx();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  return result.length;
}


