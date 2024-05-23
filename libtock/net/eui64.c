#include "eui64.h"

returncode_t libtock_eui64_get(uint64_t* eui64) {
  return libtock_eui64_command_get(eui64);
}
