#include <libtock/defer.h>
#include <libtock/peripherals/syscalls/spi_peripheral_syscalls.h>

#include "spi_peripheral.h"

#include "syscalls/spi_peripheral_syscalls.h"

bool libtocksync_spi_peripheral_exists(void) {
  return libtock_spi_peripheral_driver_exists();
}

returncode_t libtocksync_spi_peripheral_write(const uint8_t* write,
                                              size_t         len) {
  returncode_t err;

  err = libtock_spi_peripheral_allow_readonly_write((uint8_t*) write, len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_spi_peripheral_allow_readonly_write(NULL, 0);
  }

  err = libtock_spi_peripheral_command_write(len);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_spi_peripheral_yield_wait_for();
  return err;
}

returncode_t libtocksync_spi_peripheral_read_write(const uint8_t* write,
                                                   uint8_t*       read,
                                                   size_t         len) {
  returncode_t err;

  err = libtock_spi_peripheral_allow_readwrite_read(read, len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_spi_peripheral_allow_readwrite_read(NULL, 0);
  }

  err = libtock_spi_peripheral_allow_readonly_write((uint8_t*) write, len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_spi_peripheral_allow_readonly_write(NULL, 0);
  }

  err = libtock_spi_peripheral_command_write(len);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_spi_peripheral_yield_wait_for();
  return err;
}
