#pragma once

#include "../tock.h"
#include "crc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for CRC callback.
//
// - `arg1` (`returncode_t`): Status of the crc.
// - `arg2` (`uint32_t`): CRC result.
typedef void (*libtock_crc_callback_computed)(returncode_t, uint32_t);

// Check if the driver exists.
bool libtock_crc_exists(void);

// Compute a CRC value over the given buffer using the given algorithm.
returncode_t libtock_crc_compute(const uint8_t* buf, uint32_t buflen, libtock_crc_alg_t algorithm,
                                 libtock_crc_callback_computed cb);

#ifdef __cplusplus
}
#endif
