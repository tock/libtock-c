#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "../tock.h"

#define BUTTON1 0
#define BUTTON2 1
#define BUTTON3 2

/*
 * Command to check if the app loader driver exists.
 */
bool libtock_app_loader_exists(void);

/*
 * Command to request the kernel to set up for a new app.
 * This functions takes the size of the new app as the argument.
 */
returncode_t libtock_app_loader_setup(uint32_t app_length, subscribe_upcall cb);

/*
 * Command to write new app binary to the capsule.
 * Specify the offset at which the data segment should be written.
 * Also specify how long the data segment will be in bytes.
 */
returncode_t libtock_app_loader_write(uint32_t offset, uint8_t* chunk_data, size_t chunk_len);

/*
 * Signal to the app we are done writing a process binary.
 */
returncode_t libtock_app_loader_finalize(subscribe_upcall cb);

/*
 * Command to request the kernel to load the newly flashed app.
 */
returncode_t libtock_app_loader_load(subscribe_upcall cb);

/*
 * Command to request the kernel to abort the setup/write phase.
 */
returncode_t libtock_app_loader_abort(subscribe_upcall cb);

/*
 * Command to request the kernel to unload a running process identified
 * by its ShortID.
 */
returncode_t libtock_app_loader_unload(uint32_t app_short_id, subscribe_upcall cb);

/*
 * Command to request the kernel to uninstall an application binary identified by an
 * app handle (returned by the unload function).
 */
returncode_t libtock_app_loader_uninstall_with_app_handle(uint32_t app_handle, subscribe_upcall cb);

/*
 * Internal function to write new app to flash. Takes app size as argument.
 */
int write_app(double size, uint8_t binary[]);



#ifdef __cplusplus
}
#endif
