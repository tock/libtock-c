#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_APP_STATE 0x50000

// Check if the app state system call driver is available on this board.
bool libtock_app_state_driver_exists(void);

// Configure the upcall for the app state driver.
returncode_t libtock_app_state_set_upcall(subscribe_upcall cb, void* opaque);

// Share the buffer with the app state driver.
returncode_t libtock_app_state_set_readonly_allow(const uint8_t* ptr, uint32_t size);

// Tell the driver to write the buffer to the flash storage.
returncode_t libtock_app_state_command_save(uint32_t flash_pointer);

#ifdef __cplusplus
}
#endif
