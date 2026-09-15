#pragma once

#include "../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for registration complete callback.
//
// - `arg1` (`returncode_t`): Status from registration.
typedef void (*libtock_ipc_registry_string_name_registration_complete_callback)(returncode_t);

// Function signature for discovery complete callback.
//
// - `arg1` (`returncode_t`): Status from discovery.
// - `arg2` (`uint64_t`): IPC identifier for discovered service.
typedef void (*libtock_ipc_registry_string_name_discovery_complete_callback)(returncode_t, uint64_t);


// Check if the driver exists.
bool libtock_ipc_registry_string_name_exists(void);

// Register as a service using a string name.
//
// If successful, the callback will be received when registration has completed.
//
// ## Arguments
//
// `cb`: Callback function triggered when registration is completed.
// `string_name`: Buffer of uint8_t values to use as service name
// `string_name_len`: Length of buffer.
//
// ## Return value
//
// A returncode indicating whether the registration process started successfully.
returncode_t libtock_ipc_registry_string_name_register_service(
  libtock_ipc_registry_string_name_registration_complete_callback cb,
  uint8_t*                                                        string_name,
  size_t                                                          string_name_len);

// Discover a service using a string name.
//
// If successful, the callback will be triggered when discovery has completed.
//
// ## Arguments
//
// `cb`: Callback function triggered when registration is completed.
// `string_name`: Buffer of uint8_t values to use as service name
// `string_name_len`: Length of buffer.
//
// ## Return value
//
// A returncode indicating whether the registration process started successfully.
returncode_t libtock_ipc_registry_string_name_discover_service(
  libtock_ipc_registry_string_name_discovery_complete_callback cb,
  uint8_t*                                                     string_name,
  size_t                                                       string_name_len);

#ifdef __cplusplus
}
#endif
