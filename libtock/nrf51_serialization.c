#include "nrf51_serialization.h"

// #define NRF51_SERIALIZATION_COMMAND_CHECK 0
#define NRF51_SERIALIZATION_COMMAND_WRITE 1
#define NRF51_SERIALIZATION_COMMAND_READ  2
#define NRF51_SERIALIZATION_COMMAND_RESET 3


int nrf51_serialization_reset (void) {
  // Reset the nRF51 chip
  syscall_return_t sval = command(DRIVER_NUM_NRF_SERIALIZATION,
                                  NRF51_SERIALIZATION_COMMAND_RESET,
                                  0, 0);
  if (sval.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else {
    return TOCK_FAIL;
  }
}

int nrf51_serialization_subscribe (subscribe_upcall cb) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_NRF_SERIALIZATION, 0, cb, NULL);
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



int nrf51_serialization_write(char* tx, int tx_len) {
  // Pass in the TX buffer.
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_NRF_SERIALIZATION, 0, tx, tx_len);
  if (!aval.success) {
    return tock_error_to_rcode(aval.error);
  }

  // Write the data.
  syscall_return_t sval = command(DRIVER_NUM_NRF_SERIALIZATION,
                                  NRF51_SERIALIZATION_COMMAND_WRITE,
                                  0, 0);
  if (sval.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode(sval.data[0]);
  }
}

int nrf51_serialization_read(int rx_len) {
  syscall_return_t sval = command(DRIVER_NUM_NRF_SERIALIZATION,
                                  NRF51_SERIALIZATION_COMMAND_READ, rx_len, 0);
  if (sval.type == TOCK_SYSCALL_SUCCESS_U32) {
    return sval.data[0]; // Actual read length
  } else if (sval.type == TOCK_SYSCALL_FAILURE) {
    return tock_error_to_rcode(sval.data[0]);
  } else {
    return TOCK_EBADRVAL;
  }
}
