#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "libtock/tock.h"

// Driver number for system call
#define DRIVER_NUM_IN_PLACE_PROCESS_LOADER   0x10002

#define BUTTON1 0

/*
 * Command to check if the `in_place_process_loader` capsule exists.
 */
int in_place_process_loader_exists(void);

/*
 * Command to request the kernel to load the newly flashed app.
 */
int in_place_process_loader_command_load(uint32_t app_address, uint32_t app_size);

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
int in_place_process_loader_load_subscribe(subscribe_upcall cb, void* userdata);


#ifdef __cplusplus
}
#endif
