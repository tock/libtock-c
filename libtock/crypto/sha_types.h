#pragma once

#include "../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  LIBTOCK_SHA256 = 0,
  LIBTOCK_SHA384 = 1,
  LIBTOCK_SHA512 = 2,
} libtock_sha_algorithm_t;

#ifdef __cplusplus
}
#endif
