#include "spi_controller_syscalls.h"

bool libtock_spi_controller_driver_exists(void) {
  return driver_exists(DRIVER_NUM_SPI_CONTROLLER);
}

returncode_t libtock_spi_controller_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_SPI_CONTROLLER, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_spi_controller_allow_readwrite_read(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_SPI_CONTROLLER, 0, buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

returncode_t libtock_spi_controller_allow_readonly_write(uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_SPI_CONTROLLER, 0, buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_spi_controller_command_write_byte(uint32_t byte) {
  syscall_return_t cval = command(DRIVER_NUM_SPI_CONTROLLER, 1, byte, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_spi_controller_command_read_write_bytes(uint32_t len) {
  syscall_return_t cval = command(DRIVER_NUM_SPI_CONTROLLER, 2, len, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_spi_controller_command_set_chip_select(uint32_t chip_select) {
  syscall_return_t cval = command(DRIVER_NUM_SPI_CONTROLLER, 3, chip_select, 0);
  return tock_command_return_novalue_to_returncode(cval);
}


returncode_t libtock_spi_controller_command_get_chip_select(uint32_t* chip_select) {
  syscall_return_t cval = command(DRIVER_NUM_SPI_CONTROLLER, 4, 0, 0);
  return tock_command_return_u32_to_returncode(cval, chip_select);
}

returncode_t libtock_spi_controller_command_set_rate(uint32_t rate) {
  syscall_return_t cval = command(DRIVER_NUM_SPI_CONTROLLER, 5, rate, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_spi_controller_command_get_rate(uint32_t* rate) {
  syscall_return_t cval = command(DRIVER_NUM_SPI_CONTROLLER, 6, 0, 0);
  return tock_command_return_u32_to_returncode(cval, rate);
}

returncode_t libtock_spi_controller_command_set_phase(bool phase) {
  syscall_return_t cval = command(DRIVER_NUM_SPI_CONTROLLER, 7, (unsigned char)phase, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_spi_controller_command_get_phase(uint32_t* phase) {
  syscall_return_t cval = command(DRIVER_NUM_SPI_CONTROLLER, 8, 0, 0);
  return tock_command_return_u32_to_returncode(cval, phase);
}

returncode_t libtock_spi_controller_command_set_polarity(bool polarity) {
  syscall_return_t cval = command(DRIVER_NUM_SPI_CONTROLLER, 9, (unsigned char) polarity, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_spi_controller_command_get_polarity(uint32_t* polarity) {
  syscall_return_t cval = command(DRIVER_NUM_SPI_CONTROLLER, 10, 0, 0);
  return tock_command_return_u32_to_returncode(cval, polarity);
}

returncode_t libtock_spi_controller_command_hold_low(void) {
  syscall_return_t cval = command(DRIVER_NUM_SPI_CONTROLLER, 11, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_spi_controller_command_release_low(void) {
  syscall_return_t cval = command(DRIVER_NUM_SPI_CONTROLLER, 12, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
