#include "moisture.h"

#include "syscalls/moisture_syscalls.h"

bool libtocksync_moisture_exists(void) {
  return libtock_moisture_driver_exists();
}

returncode_t libtocksync_moisture_read(int* moisture) {
  returncode_t err;

  err = libtock_moisture_command_read();
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_moisture_yield_wait_for(moisture);
  return err;
}
