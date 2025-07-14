#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "../tock.h"

#define BUTTON1 0
#define BUTTON2 1
#define BUTTON3 2

// Check if the driver exists.
bool libtock_app_loader_exists(void);

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
returncode_t libtock_app_loader_subscribe_setup(subscribe_upcall cb, void* userdata);

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
returncode_t libtock_app_loader_subscribe_write(subscribe_upcall cb, void* userdata);

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
returncode_t libtock_app_loader_subscribe_finalize(subscribe_upcall cb, void* userdata);

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
returncode_t libtock_app_loader_subscribe_load(subscribe_upcall cb, void* userdata);

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
returncode_t libtock_app_loader_subscribe_abort(subscribe_upcall cb, void* userdata);

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
 * Signal to the app we are done writing a process binary.
 */
returncode_t libtock_app_loader_finalize(void);

/*
 * Command to request the kernel to load the newly flashed app.
 */
returncode_t libtock_app_loader_load(void);

/*
 * Command to request the kernel to abort the setup/write phase.
 */
returncode_t libtock_app_loader_abort(void);


/*
 * Internal function to write new app to flash. Takes app size as argument.
 */
int write_app(double size, uint8_t binary[]);



#ifdef __cplusplus
}
#endif
