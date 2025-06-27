#include "temperature.h"

returncode_t libtocksync_temperature_read(int* temperature) {
  returncode_t err;

  err = libtock_temperature_command_read();
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the operation to finish.
  err = libtocksync_temperature_yield_wait_for(temperature);

  return err;
}
