#pragma once

#include <libtock/crypto/syscalls/hmac_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for an HMAC operation to finish.
returncode_t libtocksync_hmac_yield_wait_for(void);

#ifdef __cplusplus
}
#endif
