#pragma once

#include <libtock/crypto/syscalls/sha_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for an SHA hash to finish.
returncode_t libtocksync_sha_yield_wait_for_hash(void);

// Wait for an SHA verify to finish.
returncode_t libtocksync_sha_yield_wait_for_verify(bool* correct);

#ifdef __cplusplus
}
#endif
