#pragma once

#include "../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  LIBTOCK_HMAC_SHA256 = 0,
  LIBTOCK_HMAC_SHA384 = 1,
  LIBTOCK_HMAC_SHA512 = 2,
} libtock_hmac_algorithm_t;

#ifdef __cplusplus
}
#endif
