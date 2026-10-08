#pragma once

#include <libtock/ipc/ipc_registry_string_name.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Determine if the IPC Registry String Name capsule exists.
//
// ## Return Value
//
// True if the capsule exists.
bool libtocksync_ipc_registry_string_name_exists(void);

// Register a new service by string name.
//
// ## Arguments
//
// - `string_name`: Array of uint8_t with the string name of the service to be registered.
// - `string_name_len`: Length of the array.
//
// ## Return Value
//
// A returncode indicating whether the service was registered successfully.
returncode_t libtocksync_ipc_registry_string_name_register_service(uint8_t* string_name, size_t string_name_len);

// Discovery a service by string name.
//
// ## Arguments
//
// - `string_name`: Array of uint8_t with the string name of the service to be discovered.
// - `string_name_len`: Length of the array.
// - `ipc_id`: Pointer to a location for the discovered IPC identifier to be stored.
//
// ## Return Value
//
// A returncode indicating whether the service was discovered successfully.
returncode_t libtocksync_ipc_registry_string_name_discover_service(uint8_t* string_name, size_t string_name_len,
                                                                   uint64_t* ipc_id);


#ifdef __cplusplus
}
#endif
