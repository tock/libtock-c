
#include <libtock/defer.h>

#include "serial.h"

#define DRIVER_NUM_YMODEM 0xA0000





static returncode_t libtock_ymodem_set_read_allow(const uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_YMODEM, 1, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

static returncode_t libtock_ymodem_set_readwrite_allow(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_YMODEM, 1, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}



static returncode_t libtock_ymodem_command_write(int length) {
  syscall_return_t cval = command(DRIVER_NUM_YMODEM, 1, length, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

static returncode_t libtock_ymodem_command_read(int length) {
  syscall_return_t cval = command(DRIVER_NUM_YMODEM, 2, length, 0);
  return tock_command_return_novalue_to_returncode(cval);
}




static returncode_t libtocksync_ymodem_yield_wait_for_write(uint32_t* bytes_written) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_YMODEM, 1);

  *bytes_written = ret.data1;

  return tock_status_to_returncode(ret.data0);
}

static returncode_t libtocksync_ymodem_yield_wait_for_read(uint32_t* bytes_read) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_YMODEM, 2);

  *bytes_read = ret.data1;

  return tock_status_to_returncode(ret.data0);
}

bool libtock_ymodem_driver_exists(void) {
  return driver_exists(DRIVER_NUM_YMODEM);
}

returncode_t libtocksync_ymodem_write(const uint8_t* buffer, uint32_t length, uint32_t* written) {
  int err;

  err = libtock_ymodem_set_read_allow(buffer, length);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_ymodem_set_read_allow(NULL, 0);
  }

  err = libtock_ymodem_command_write((int) length);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_ymodem_yield_wait_for_write(written);
  return err;
}

returncode_t libtocksync_ymodem_read(uint8_t* buffer, uint32_t length, uint32_t* read) {
  int err;

  err = libtock_ymodem_set_readwrite_allow(buffer, length);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_ymodem_set_readwrite_allow(NULL, 0);
  }

  err = libtock_ymodem_command_read((int) length);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_ymodem_yield_wait_for_read(read);
  return err;
}

