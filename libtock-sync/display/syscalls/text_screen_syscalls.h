#pragma once

#include <libtock/display/syscalls/text_screen_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a text screen operation to complete.
returncode_t libtocksync_text_screen_yield_wait_for_done(void);

// Wait for a text screen get size operation to complete.
returncode_t libtocksync_text_screen_yield_wait_for_size(uint32_t* width, uint32_t* height);

#ifdef __cplusplus
}
#endif
