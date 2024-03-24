#pragma once

#include <tock.h>
#include <libtock/crc.h>

#ifdef __cplusplus
extern "C" {
#endif

// Compute a CRC.
//
// Returns `SUCCESS` and sets `result` on success.
// Returns `EBUSY` if a computation is already in progress.
// Returns `ESIZE` if the buffer is too big for the unit.
returncode_t libtocksync_crc_compute(const uint8_t* buf, size_t buflen, libtock_crc_alg_t algorithm, uint32_t* result);

#ifdef __cplusplus
}
#endif
