#pragma once

#include <libtock/ipc/ipc_registry_package_name.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Determine if the IPC Registry Package Name capsule exists.
//
// ## Return Value
//
// True if the capsule exists.
bool libtocksync_ipc_registry_package_name_exists(void);

// Register a new service by package name.
//
// ## Return Value
//
// A returncode indicating whether the service was registered successfully.
returncode_t libtocksync_ipc_registry_package_name_register_service(void);

// Discovery a service by package name.
//
// ## Arguments
//
// - `package_name`: Array of uint8_t with the package name of the service to be discovered.
// - `package_name_len`: Length of the array.
// - `ipc_id`: Pointer to a location for the discovered IPC identifier to be stored.
//
// ## Return Value
//
// A returncode indicating whether the service was discovered successfully.
returncode_t libtocksync_ipc_registry_package_name_discover_service(uint8_t* package_name, size_t package_name_len,
                                                                    uint64_t* ipc_id);


#ifdef __cplusplus
}
#endif
