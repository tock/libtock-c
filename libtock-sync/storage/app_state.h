#pragma once

#include <tock.h>
#include <storage/app_state.h>

#ifdef __cplusplus
extern "C" {
#endif

// Save the application state.
returncode_t libtocksync_app_state_save(void);

#ifdef __cplusplus
}
#endif