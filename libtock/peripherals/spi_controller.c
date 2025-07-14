#include "spi_controller.h"
#include "syscalls/spi_controller_syscalls.h"

bool libtock_spi_controller_exists(void) {
  return libtock_spi_controller_driver_exists();
}

returncode_t libtock_spi_controller_set_chip_select(uint32_t chip_select) {
  return libtock_spi_controller_command_set_chip_select(chip_select);
}

returncode_t libtock_spi_controller_get_chip_select(uint32_t* chip_select) {
  return libtock_spi_controller_command_get_chip_select(chip_select);
}

returncode_t libtock_spi_controller_set_rate(uint32_t rate) {
  return libtock_spi_controller_command_set_rate(rate);
}

returncode_t libtock_spi_controller_get_rate(uint32_t* rate) {
  return libtock_spi_controller_command_get_rate(rate);
}

returncode_t libtock_spi_controller_set_phase(bool phase) {
  return libtock_spi_controller_command_set_phase(phase);
}

returncode_t libtock_spi_controller_get_phase(uint32_t* phase) {
  return libtock_spi_controller_command_get_phase(phase);
}

returncode_t libtock_spi_controller_set_polarity(bool polarity) {
  return libtock_spi_controller_command_set_polarity(polarity);
}

returncode_t libtock_spi_controller_get_polarity(uint32_t* polarity) {
  return libtock_spi_controller_command_get_polarity(polarity);
}

returncode_t libtock_spi_controller_hold_low(void) {
  return libtock_spi_controller_command_hold_low();
}

returncode_t libtock_spi_controller_release_low(void) {
  return libtock_spi_controller_command_release_low();
}

returncode_t libtock_spi_controller_write_byte(unsigned char byte) {
  return libtock_spi_controller_command_write_byte((uint32_t) byte);
}

static void spi_controller_upcall(__attribute__ ((unused)) int unused0,
                                  __attribute__ ((unused)) int unused1,
                                  __attribute__ ((unused)) int unused2,
                                  void*                        opaque) {
  libtock_spi_controller_callback cb = (libtock_spi_controller_callback) opaque;
  cb(RETURNCODE_SUCCESS);
}

returncode_t libtock_spi_controller_write(const uint8_t*                  buffer,
                                          size_t                          len,
                                          libtock_spi_controller_callback cb) {
  returncode_t ret;

  ret = libtock_spi_controller_allow_readonly_write((uint8_t*) buffer, len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_spi_controller_set_upcall(spi_controller_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_spi_controller_command_read_write_bytes(len);
  return ret;
}

returncode_t libtock_spi_controller_read_write(const uint8_t*                  write,
                                               uint8_t*                        read,
                                               size_t                          len,
                                               libtock_spi_controller_callback cb) {
  returncode_t ret;

  ret = libtock_spi_controller_allow_readwrite_read(read, len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  return libtock_spi_controller_write(write, len, cb);
}
