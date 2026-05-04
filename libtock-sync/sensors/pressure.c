#include "pressure.h"

#include "syscalls/pressure_syscalls.h"

bool libtocksync_pressure_exists(void) {
  return libtock_pressure_driver_exists();
}

returncode_t libtocksync_pressure_read(int* pressure) {
  returncode_t err;

  err = libtock_pressure_command_read();
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_pressure_yield_wait_for(pressure);
  return err;
}
