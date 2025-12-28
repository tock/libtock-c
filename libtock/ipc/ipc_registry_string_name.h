#pragma once

#include "../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for registration complete callback.
//
// - `arg1` (`returncode_t`): Status from registration.
// - `arg2` (`bool`): Registration succeeded.
typedef void (*libtock_ipc_registry_string_name_registration_complete_callback)(returncode_t, bool);


// Check if the driver exists.
bool libtock_ipc_registry_string_name_exists(void);

// Register as a service using a string name.
//
// The callback will be triggered to indicate success or failure.
// returncode_t libtock_register_service(
//   libtock_ipc_registry_string_name_registration_complete_callback cb,
//   uint8_t*                                                        string_name,
//   uint8_t                                                         string_name_len);

// TODO FILL IN THE REST OF THIS

#ifdef __cplusplus
}
#endif
