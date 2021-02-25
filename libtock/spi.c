#include "spi.h"

__attribute__((const))
int spi_init(void) {
  return 0;
}
int spi_set_chip_select(unsigned char cs) {
  syscall_return_t comval = command2(DRIVER_NUM_SPI, 3, cs, 0);
  if (comval.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(comval.data[0]);
  } else {
    return TOCK_SUCCESS;
  }
}
int spi_get_chip_select(void) {
  syscall_return_t comval = command2(DRIVER_NUM_SPI, 4, 0, 0);
  if (comval.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(comval.data[0]);
  } else {
    return TOCK_SUCCESS;
  }
}
int spi_set_rate(int rate) {
  syscall_return_t comval = command2(DRIVER_NUM_SPI, 5, rate, 0);
  if (comval.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(comval.data[0]);
  } else {
    return TOCK_SUCCESS;
  }
}
int spi_get_rate(void) {
  syscall_return_t comval = command2(DRIVER_NUM_SPI, 6, 0, 0);
  if (comval.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(comval.data[0]);
  } else {
    return TOCK_SUCCESS;
  }
}
int spi_set_phase(bool phase) {
  syscall_return_t comval = command2(DRIVER_NUM_SPI, 7, (unsigned char)phase, 0);
  if (comval.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(comval.data[0]);
  } else {
    return TOCK_SUCCESS;
  }
}
int spi_get_phase(void) {
  syscall_return_t comval = command2(DRIVER_NUM_SPI, 8, 0, 0);
  if (comval.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(comval.data[0]);
  } else {
    return TOCK_SUCCESS;
  }
}
int spi_set_polarity(bool pol) {
  syscall_return_t comval = command2(DRIVER_NUM_SPI, 9, (unsigned char)pol, 0);
  if (comval.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(comval.data[0]);
  } else {
    return TOCK_SUCCESS;
  }
}
int spi_get_polarity(void) {
  syscall_return_t comval = command2(DRIVER_NUM_SPI, 10, 0, 0);
  if (comval.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(comval.data[0]);
  } else {
    return TOCK_SUCCESS;
  }
}
int spi_hold_low(void) {
  syscall_return_t comval = command2(DRIVER_NUM_SPI, 11, 0, 0);
  if (comval.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(comval.data[0]);
  } else {
    return TOCK_SUCCESS;
  }
}
int spi_release_low(void) {
  syscall_return_t comval = command2(DRIVER_NUM_SPI, 12, 0, 0);
  if (comval.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(comval.data[0]);
  } else {
    return TOCK_SUCCESS;
  }
}
int spi_write_byte(unsigned char byte) {
  syscall_return_t comval = command2(DRIVER_NUM_SPI, 1, byte, 0);
  if (comval.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(comval.data[0]);
  } else {
    return TOCK_SUCCESS;
  }
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
  allow_ro_return_t allowval = allow_readonly(DRIVER_NUM_SPI, 0, buf, len);
  if (allowval.success == 0 ) {
    return tock_error_to_rcode(allowval.error);
  }
  subscribe_return_t subval = subscribe2(DRIVER_NUM_SPI, 0, cb, cond);
  if (subval.success == 0) {
    return tock_error_to_rcode(subval.error);
  }
  syscall_return_t comval = command2(DRIVER_NUM_SPI, 2, len, 0);
  if (comval.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(comval.data[0]);
  } else {
    return TOCK_SUCCESS;
  }
}

int spi_read_write(const char* write,
                   char* read,
                   size_t len,
                   subscribe_upcall cb, bool* cond) {

  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_SPI, 0, (void*)read, len);
  if (aval.success == 0) {
    return tock_error_to_rcode(aval.error);
  }
  return spi_write(write, len, cb, cond);
}

int spi_write_sync(const char* write,
                   size_t len) {
  bool cond = false;
  spi_write(write, len, spi_upcall, &cond);
  yield_for(&cond);
  return 0;
}

int spi_read_write_sync(const char* write,
                        char* read,
                        size_t len) {
  bool cond = false;
  int err   = spi_read_write(write, read, len, spi_upcall, &cond);
  if (err < 0) {
    return err;
  }
  yield_for(&cond);
  return 0;
}
