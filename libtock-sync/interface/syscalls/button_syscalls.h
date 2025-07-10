#pragma once

#include <libtock/interface/syscalls/button_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a button upcall.
//
// - `button_index`: Button index.
// - `pressed`: True if pressed, false otherwise.
returncode_t libtocksync_button_yield_wait_for(int* button_index, bool* pressed);

#ifdef __cplusplus
}
#endif
