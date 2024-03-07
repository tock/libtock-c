#include "spi_peripheral_syscalls.h"

bool libtock_spi_peripheral_exists(void) {
  return driver_exists(DRIVER_NUM_SPI_PERIPHERAL);
}

returncode_t libtock_spi_peripheral_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_SPI_PERIPHERAL, 1, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_spi_peripheral_allow_readwrite_read(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_SPI_PERIPHERAL, 0, buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

returncode_t libtock_spi_peripheral_allow_readonly_write(uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_SPI_PERIPHERAL, 0, buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_spi_peripheral_command_write(uint32_t len) {
  syscall_return_t cval = command(DRIVER_NUM_SPI_PERIPHERAL, 1, len, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_spi_peripheral_command_get_chip_select(uint32_t* chip_select) {
  syscall_return_t cval = command(DRIVER_NUM_SPI_PERIPHERAL, 2, 0, 0);
  return tock_command_return_u32_to_returncode(cval, chip_select);
}

returncode_t libtock_spi_peripheral_command_set_phase(bool phase) {
  syscall_return_t cval = command(DRIVER_NUM_SPI_PERIPHERAL, 3, phase, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_spi_peripheral_command_get_phase(uint32_t* phase) {
  syscall_return_t cval = command(DRIVER_NUM_SPI_PERIPHERAL, 4, 0, 0);
  return tock_command_return_u32_to_returncode(cval, phase);
}

returncode_t libtock_spi_peripheral_command_set_polarity(bool polarity) {
  syscall_return_t cval = command(DRIVER_NUM_SPI_PERIPHERAL, 5, polarity, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_spi_peripheral_command_get_polarity(uint32_t* polarity) {
  syscall_return_t cval = command(DRIVER_NUM_SPI_PERIPHERAL, 6, 0, 0);
  return tock_command_return_u32_to_returncode(cval, polarity);
}
