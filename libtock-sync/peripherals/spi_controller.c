#include <libtock/defer.h>
#include <libtock/peripherals/syscalls/spi_controller_syscalls.h>

#include "spi_controller.h"

#include "syscalls/spi_controller_syscalls.h"

bool libtocksync_spi_controller_exists(void) {
  return libtock_spi_controller_driver_exists();
}

returncode_t libtocksync_spi_controller_write(const uint8_t* write,
                                              size_t         len) {
  returncode_t err;

  err = libtock_spi_controller_allow_readonly_write((uint8_t*) write, len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_spi_controller_allow_readonly_write(NULL, 0);
  }

  err = libtock_spi_controller_command_read_write_bytes(len);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_spi_controller_yield_wait_for();
  return err;
}

returncode_t libtocksync_spi_controller_read_write(const uint8_t* write,
                                                   uint8_t*       read,
                                                   size_t         len) {
  returncode_t err;

  err = libtock_spi_controller_allow_readwrite_read(read, len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_spi_controller_allow_readwrite_read(NULL, 0);
  }

  err = libtock_spi_controller_allow_readonly_write((uint8_t*) write, len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_spi_controller_allow_readonly_write(NULL, 0);
  }

  err = libtock_spi_controller_command_read_write_bytes(len);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_spi_controller_yield_wait_for();
  return err;
}
