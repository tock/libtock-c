#include "spi_peripheral.h"
#include "syscalls/spi_peripheral_syscalls.h"

bool libtock_spi_peripheral_exists(void) {
  return libtock_spi_peripheral_driver_exists();
}

// Return the chip select. This will always return 0.
returncode_t libtock_spi_peripheral_get_chip_select(uint32_t* chip_select) {
  return libtock_spi_peripheral_command_get_chip_select(chip_select);
}


returncode_t libtock_spi_peripheral_get_phase(uint32_t* phase) {
  return libtock_spi_peripheral_command_get_phase(phase);
}


returncode_t libtock_spi_peripheral_set_phase(bool phase) {
  return libtock_spi_peripheral_command_set_phase(phase);
}


returncode_t libtock_spi_peripheral_get_polarity(uint32_t* polarity) {
  return libtock_spi_peripheral_command_get_polarity(polarity);
}


returncode_t libtock_spi_peripheral_set_polarity(bool polarity) {
  return libtock_spi_peripheral_command_set_polarity(polarity);
}

static void spi_peripheral_upcall(__attribute__ ((unused)) int unused0,
                                  __attribute__ ((unused)) int unused1,
                                  __attribute__ ((unused)) int unused2,
                                  void*                        opaque) {
  libtock_spi_peripheral_callback cb = (libtock_spi_peripheral_callback) opaque;
  cb(RETURNCODE_SUCCESS);
}

returncode_t libtock_spi_peripheral_write(const uint8_t*                  buffer,
                                          size_t                          len,
                                          libtock_spi_peripheral_callback cb) {
  returncode_t ret;

  ret = libtock_spi_peripheral_allow_readonly_write((uint8_t*) buffer, len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_spi_peripheral_set_upcall(spi_peripheral_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_spi_peripheral_command_write(len);
  return ret;
}

returncode_t libtock_spi_peripheral_read_write(const uint8_t*                  write,
                                               uint8_t*                        read,
                                               size_t                          len,
                                               libtock_spi_peripheral_callback cb) {
  returncode_t ret;

  ret = libtock_spi_peripheral_allow_readwrite_read(read, len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  return libtock_spi_peripheral_write(write, len, cb);
}
