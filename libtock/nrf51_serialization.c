#include "nrf51_serialization.h"

int nrf51_serialization_reset (void) {
  // Reset the nRF51 chip
  syscall_return_t sval = command2(DRIVER_NUM_NRF_SERIALIZATION, 3, 0, 0);
  if (sval.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else {
    return TOCK_FAIL;
  }
}

int nrf51_serialization_subscribe (subscribe_cb cb) {
  // get some callback love
  subscribe_return_t sval = subscribe2(DRIVER_NUM_NRF_SERIALIZATION, 0, cb, NULL);
  if (!sval.success) {
    return tock_error_to_rcode(sval.error);
  }

  // Instruct the capsule to start receiving. Doesn't
  syscall_return_t cval = command2(DRIVER_NUM_NRF_SERIALIZATION, 2, 0, 0);
  if (cval.type == TOCK_SYSCALL_SUCCESS_U32) {
    return TOCK_SUCCESS;
  } else if (cval.type == TOCK_SYSCALL_FAILURE) {
    // Ok if we get ERESERVE. Just means a buffer hasn't been set.
    if ((int) cval.data[0] == TOCK_ERESERVE) {
      return TOCK_SUCCESS;
    }
  }
  return tock_error_to_rcode(cval.data[0]);
}

int nrf51_serialization_setup_rx_buffer (char* rx, int rx_len) {
  // Pass the RX buffer for the UART module to use.
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_NRF_SERIALIZATION, 0, rx, rx_len);
  if (aval.success) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode(aval.error);
  }
}

int nrf51_serialization_write (char* tx, int tx_len) {
  // Pass in the TX buffer.
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_NRF_SERIALIZATION, 0, tx, tx_len);
  if (!aval.success) {
    return tock_error_to_rcode(aval.error);
  }

  // Do the write!!!!!
  syscall_return_t sval = command2(DRIVER_NUM_NRF_SERIALIZATION, 1, 0, 0);
  if (sval.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode(sval.data[0]);
  }
}
