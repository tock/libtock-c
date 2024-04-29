#pragma once

#include "tock.h"
#include "syscalls/eui64_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

returncode_t libtock_eui64_get(uint64_t* eui64);

#ifdef __cplusplus
}
#endif
