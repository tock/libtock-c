#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_IPC_REGISTRY_STRING_NAME 0xA0000

// Check if the ipc registry string name driver exists.
bool libtock_ipc_registry_string_name_driver_exists(void);

// Configure the upcall when registration completes.
returncode_t libtock_ipc_registry_string_name_set_upcall_registration_complete(subscribe_upcall callback, void* opaque);

// Configure the upcall when discovery completes.
returncode_t libtock_ipc_registry_string_name_set_upcall_discovery_complete(subscribe_upcall callback, void* opaque);

// Configure the upcall when a new service registers.
returncode_t libtock_ipc_registry_string_name_set_upcall_new_registration(subscribe_upcall callback, void* opaque);

// Allow a buffer containing a string name for registration or discovery.
returncode_t libtock_ipc_registry_string_name_set_allow_string_name(void* data, int len);

// Register service.
returncode_t libtock_ipc_registry_string_name_command_register(void);

// Discover service.
returncode_t libtock_ipc_registry_string_name_command_discover(void);

// Enable registration notifications
returncode_t libtock_ipc_registry_string_name_command_enable_registration_notifications(void);

// Disable registration notifications
returncode_t libtock_ipc_registry_string_name_command_disable_registration_notifications(void);

#ifdef __cplusplus
}
#endif
