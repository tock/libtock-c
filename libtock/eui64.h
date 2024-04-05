#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_EUI64 0x30006
#define EUI64_CMD_GETTER 1

returncode_t get_eui64(uint64_t *eui64);
returncode_t libtock_eui64_command_getter(uint64_t *eui64);

#ifdef __cplusplus
}
#endif
