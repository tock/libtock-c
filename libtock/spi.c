#include "spi.h"

__attribute__((const))
int spi_init(void) {
  return 0;
}

int spi_set_chip_select(unsigned char cs) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 3, cs, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_get_chip_select(void) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 4, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_set_rate(int rate) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 5, rate, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_get_rate(void) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 6, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_set_phase(bool phase) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 7, (unsigned char)phase, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_get_phase(void) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 8, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_set_polarity(bool pol) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 9, (unsigned char)pol, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_get_polarity(void) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 10, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_hold_low(void) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 11, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_release_low(void) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 12, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_write_byte(unsigned char byte) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 1, byte, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

static void spi_upcall(__attribute__ ((unused)) int unused0,
                       __attribute__ ((unused)) int unused1,
                       __attribute__ ((unused)) int unused2,
                       __attribute__ ((unused)) void* ud) {
  *((bool*)ud) = true;
}

int spi_write(const char* buf,
              size_t len,
              subscribe_upcall cb, bool* cond) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_SPI, 0, buf, len);
  if (aval.success == 0 ) return tock_status_to_returncode(aval.status);

  subscribe_return_t sval = subscribe(DRIVER_NUM_SPI, 0, cb, cond);
  if (sval.success == 0) return tock_status_to_returncode(sval.status);

  syscall_return_t cval = command(DRIVER_NUM_SPI, 2, len, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_read_write(const char* write,
                   char* read,
                   size_t len,
                   subscribe_upcall cb, bool* cond) {

  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_SPI, 0, (void*)read, len);
  if (aval.success == 0) return tock_status_to_returncode(aval.status);

  return spi_write(write, len, cb, cond);
}

int spi_write_sync(const char* write,
                   size_t len) {
  bool cond = false;

  int err = spi_write(write, len, spi_upcall, &cond);
  if (err < 0) return err;

  yield_for(&cond);
  return RETURNCODE_SUCCESS;
}

int spi_read_write_sync(const char* write,
                        char* read,
                        size_t len) {
  bool cond = false;

  int err = spi_read_write(write, read, len, spi_upcall, &cond);
  if (err < 0) return err;

  yield_for(&cond);
  return RETURNCODE_SUCCESS;
}
