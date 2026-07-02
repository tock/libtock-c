#pragma once

#include <libtock/interface/syscalls/console_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

returncode_t libtocksync_console_yield_wait_for_write(uint32_t* bytes_written);
returncode_t libtocksync_console_yield_wait_for_read(uint32_t* bytes_read);

#ifdef __cplusplus
}
#endif
