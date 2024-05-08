#include "nrf51_serialization_syscalls.h"

// #define NRF51_SERIALIZATION_COMMAND_CHECK 0
#define NRF51_SERIALIZATION_COMMAND_WRITE 1
#define NRF51_SERIALIZATION_COMMAND_READ  2
#define NRF51_SERIALIZATION_COMMAND_RESET 3


bool libtock_nrf51_serialization_exists(void) {
  return driver_exists(DRIVER_NUM_NRF51_SERIALIZATION);
}

returncode_t libtock_nrf51_serialization_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_NRF51_SERIALIZATION, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_nrf51_serialization_set_readwrite_allow_receive_buffer(uint8_t* rx, uint32_t rx_len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_NRF51_SERIALIZATION, 0, rx, rx_len);
  return tock_allow_rw_return_to_returncode(aval);
}

returncode_t libtock_nrf51_serialization_set_readonly_allow_write_buffer(uint8_t* tx, uint32_t tx_len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_NRF51_SERIALIZATION, 0, tx, tx_len);
  return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_nrf51_serialization_command_reset(void) {
  syscall_return_t cval = command(DRIVER_NUM_NRF51_SERIALIZATION,
                                  NRF51_SERIALIZATION_COMMAND_RESET,
                                  0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_nrf51_serialization_command_write(void) {
  syscall_return_t cval = command(DRIVER_NUM_NRF51_SERIALIZATION,
                                  NRF51_SERIALIZATION_COMMAND_WRITE,
                                  0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_nrf51_serialization_command_read(uint32_t rx_len, uint32_t* bytes_read) {
  syscall_return_t cval = command(DRIVER_NUM_NRF51_SERIALIZATION,
                                  NRF51_SERIALIZATION_COMMAND_READ, rx_len, 0);
  return tock_command_return_u32_to_returncode(cval, bytes_read);
}
