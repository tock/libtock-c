#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "tock.h"

// Driver number for system call
#define DRIVER_NUM_APP_LOADER 0x50001


int app_loader_exists(void);
int app_loader_subscribe(subscribe_upcall cb, void *userdata);
int app_loader_write_buffer(uint8_t *buffer, uint32_t len);
int app_loader_command_setup(int app_length);
int app_loader_command_write(int write_length);

#ifdef __cplusplus
}
#endif
