#pragma once

#include <libtock/net/syscalls/udp_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a UDP send to complete.
returncode_t libtocksync_udp_yield_wait_for_send(void);

// Wait for a UDP frame to be received.
returncode_t libtocksync_udp_yield_wait_for_recv(size_t* length);

#ifdef __cplusplus
}
#endif
