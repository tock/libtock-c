#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "libtock/kernel/syscalls/app_loader_syscalls.h"
#include "libtock/tock.h"

#define BUTTON1 0
#define BUTTON2 1

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
returncode_t libtock_app_loader_set_setup_upcall(subscribe_upcall cb, void* userdata);

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
returncode_t libtock_app_loader_set_write_upcall(subscribe_upcall cb, void* userdata);

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
returncode_t libtock_app_loader_set_load_upcall(subscribe_upcall cb, void* userdata);

/*
 * Command to request the kernel to set up for a new app.
 * This functions takes the size of the new app as the argument.
 */
returncode_t libtock_app_loader_setup(uint32_t app_length);


/*
 * Function to set up a shared buffer with the capsule.
 * This buffer is used to write data to the flash.
 */
returncode_t libtock_app_loader_set_buffer(uint8_t* buffer, uint32_t len);

/*
 * Command to write new app binary to the capsule.
 * Specify the offset at which the data segment should be written.
 * Also specify how long the data segment will be in bytes.
 *
 * After writing data, the userspace app should yield for
 * the write done callback from the capsule.
 */
returncode_t libtock_app_loader_write(uint32_t flash_offset, uint32_t write_length);

/*
 * Command to request the kernel to load the newly flashed app.
 */
returncode_t libtock_app_loader_load(void);


/*
 * Internal function to write new app to flash. Takes app size as argument.
 */
int write_app(double size, uint8_t binary[]);



#ifdef __cplusplus
}
#endif
