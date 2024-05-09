#include "nrf51_serialization.h"

returncode_t libtock_nrf51_serialization_reset(void) {
  return libtock_nrf51_serialization_command_reset();
}

returncode_t libtock_nrf51_serialization_write(uint8_t* tx, int tx_len) {
  returncode_t ret;

  ret = libtock_nrf51_serialization_set_readonly_allow_write_buffer(tx, tx_len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_nrf51_serialization_command_write();
  return ret;
}

returncode_t libtock_nrf51_serialization_read(int rx_len, int* bytes_read) {
  return libtock_nrf51_serialization_command_read(rx_len, (uint32_t*) bytes_read);
}
