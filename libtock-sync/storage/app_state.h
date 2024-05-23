#pragma once

#include <libtock/storage/app_state.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Save the application state.
returncode_t libtocksync_app_state_save(void);

#ifdef __cplusplus
}
#endif