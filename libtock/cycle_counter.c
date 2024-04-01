#include "cycle_counter.h"

bool cycle_counter_exists(void) {
  return driver_exists(DRIVER_NUM_CYCLE_COUNTER);
}

// Start the cycle counter
int cycle_counter_start(void) {
  syscall_return_t rval = command(DRIVER_NUM_CYCLE_COUNTER, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

// Get the current cycle count
int cycle_counter_read(uint64_t* count) {
  syscall_return_t rval = command(DRIVER_NUM_CYCLE_COUNTER, 2, 0, 0);
  return tock_command_return_u64_to_returncode(rval, (uint64_t*) count);
}

// Reset the cycle counter
int cycle_counter_reset(void) {
  syscall_return_t rval = command(DRIVER_NUM_CYCLE_COUNTER, 3, 0, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

// Stop the cycle counter
int cycle_counter_stop(void) {
  syscall_return_t rval = command(DRIVER_NUM_CYCLE_COUNTER, 4, 0, 0);
  return tock_command_return_novalue_to_returncode(rval);
}
