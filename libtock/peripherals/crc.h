#pragma once

#include "../tock.h"
#include "syscalls/crc_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for CRC callback.
//
// - `arg1` (`returncode_t`): Status of the crc.
// - `arg2` (`uint32_t`): CRC result.
typedef void (*libtock_crc_callback_computed)(returncode_t, uint32_t);

// CRC algorithms
//
// In all cases, input bytes are bit-reversed (i.e., consumed from LSB to MSB.)
//
// Algorithms prefixed with `SAM4L_` are native to that chip and thus require
// no software post-processing on platforms using it.
//
typedef enum {
  // Polynomial 0x04C11DB7, output reversed then inverted ("CRC-32")
  LIBTOCK_CRC_32,
  // Polynomial 0x1EDC6F41, output reversed then inverted ("CRC-32C" / "Castagnoli")
  LIBTOCK_CRC_32C,
  /// Polynomial 0x1021, no output post-processing
  LIBTOCK_CRC_16CCITT,
} libtock_crc_alg_t;

// Compute a CRC value over the given buffer using the given algorithm.
returncode_t libtock_crc_compute(const uint8_t* buf, uint32_t buflen, libtock_crc_alg_t algorithm,
                                 libtock_crc_callback_computed cb);

#ifdef __cplusplus
}
#endif
