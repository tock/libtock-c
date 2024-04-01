#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_EUI64 0x30006
#define EUI64_BUF_SIZE 8
#define EUI64_ALLOW_RW 0
#define EUI64_CMD_GETTER 1

int eui64_exists(void);
returncode_t get_eui64(uint8_t *eui64_buf);
int libtock_eui64_set_readwrite_allow(uint8_t *eui64_buf);
returncode_t libtock_eui64_command_getter(void);


#ifdef __cplusplus
}
#endif
