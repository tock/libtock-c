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
  if (sval.success) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode(sval.error);
  }
}

int nrf51_serialization_setup_receive_buffer (char* rx, int rx_len) {
  // Pass the RX buffer for the UART module to use.
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_NRF_SERIALIZATION, 0, rx, rx_len);
  if (aval.success) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode(aval.error);
  }
}



int nrf51_serialization_write_buffer(char* tx, int tx_len) {
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

int nrf51_serialization_receive(char* rx, int rx_len) {
  int rval = nrf51_serialization_setup_receive_buffer(rx, rx_len);
  if (rval != TOCK_SUCCESS) {
    return rval;
  }

  // Do the write!!!!!
  syscall_return_t sval = command2(DRIVER_NUM_NRF_SERIALIZATION, 2, 0, 0);
  if (sval.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode(sval.data[0]);
  }
}

