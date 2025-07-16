#pragma once

#include "../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

// Check if the driver exists.
bool libtock_eui64_exists(void);

returncode_t libtock_eui64_get(uint64_t* eui64);

#ifdef __cplusplus
}
#endif
