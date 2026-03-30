#pragma once

#include "../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif
