#include "syscalls/temperature_syscalls.h"
#include "temperature.h"


bool libtocksync_temperature_exists(void) {
  return libtock_temperature_driver_exists();
}

returncode_t libtocksync_temperature_read(int* temperature) {
  returncode_t err;

  err = libtock_temperature_command_read();
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the operation to finish.
  err = libtocksync_temperature_yield_wait_for(temperature);

  return err;
}
