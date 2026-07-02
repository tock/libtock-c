#include "humidity.h"

#include "syscalls/humidity_syscalls.h"

bool libtocksync_humidity_exists(void) {
  return libtock_humidity_driver_exists();
}

returncode_t libtocksync_humidity_read(int* humidity) {
  returncode_t err;

  err = libtock_humidity_command_read();
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_humidity_yield_wait_for(humidity);
  return err;
}
