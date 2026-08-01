#pragma once

#include <libtock/ipc/syscalls/ipc_registry_string_name_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for registration to complete
returncode_t libtocksync_ipc_registry_string_name_yield_wait_for_registration_complete(void);

// Wait for discovery to complete
returncode_t libtocksync_ipc_registry_string_name_yield_wait_for_discovery_complete(uint64_t* ipc_id);

#ifdef __cplusplus
}
#endif
