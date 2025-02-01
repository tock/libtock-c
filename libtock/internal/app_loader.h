#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "tock.h"

// Driver number for system call
#define DRIVER_NUM_APP_LOADER   0x10001

#define BUTTON1 0
#define BUTTON2 1

/*
 * Command to check if the `app_loader` capsule exists.
 */
int app_loader_exists(void);

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
int app_loader_setup_subscribe(subscribe_upcall cb, void *userdata);

/*
 * Command to request the kernel to set up for a new app.
 * This functions takes the size of the new app as the argument. 
 */
int app_loader_command_setup(uint32_t app_length);

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
int app_loader_write_subscribe(subscribe_upcall cb, void *userdata);

/*
 * Function to set up a shared buffer with the capsule.
 * This buffer is used to write data to the flash. 
 */
int app_loader_write_buffer(uint8_t *buffer, uint32_t len);

/*
 * Command to write new app binary to the capsule.
 * Specify the offset at which the data segment should be written.
 * Also specify how long the data segment will be in bytes.
 * 
 * After writing data, the userspace app should yield for 
 * the write done callback from the capsule.
 */
int app_loader_command_write(uint32_t flash_offset, uint32_t write_length);

/*
 * Command to request the kernel to load the newly flashed app. 
 */
int app_loader_command_load(void);

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
int app_loader_load_subscribe(subscribe_upcall cb, void *userdata);


/*
 * Internal function to write new app to flash. Takes app size as argument.
 */
int write_app(double size, uint8_t binary[]);


#ifdef __cplusplus
}
#endif
