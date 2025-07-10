#pragma once

#include <libtock/display/syscalls/screen_syscalls.h>
#include <libtock/display/screen_types.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

returncode_t libtocksync_screen_yield_wait_for(void);
returncode_t libtocksync_screen_yield_wait_for_format(libtock_screen_format_t* format);
returncode_t libtocksync_screen_yield_wait_for_rotation(libtock_screen_rotation_t* rotation);

#ifdef __cplusplus
}
#endif
