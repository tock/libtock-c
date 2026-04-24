#include "proximity.h"

#include "syscalls/proximity_syscalls.h"

bool libtocksync_proximity_exists(void) {
  return libtock_proximity_driver_exists();
}

returncode_t libtocksync_proximity_read(uint8_t* proximity) {
  returncode_t err;

  err = libtock_proximity_command_read();
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_proximity_yield_wait_for(proximity);
  return err;
}

returncode_t libtocksync_proximity_read_on_interrupt(uint32_t lower_threshold, uint32_t higher_threshold,
                                                     uint8_t* proximity) {
  returncode_t err;

  err = libtock_proximity_command_read_on_interrupt(lower_threshold, higher_threshold);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_proximity_yield_wait_for(proximity);
  return err;
}
