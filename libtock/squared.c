#include "squared.h"
#include "tock.h"

int squared_square(uint32_t number, uint64_t* squared) {
  uint32_t val0;
  syscall_return_t ret = command(DRIVER_NUM_SQUARED, 1, number, 0);
  return tock_command_return_u32_u64_to_returncode(ret, &val0, squared);
}