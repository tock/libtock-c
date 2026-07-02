#pragma once

#include <libtock/sensors/syscalls/ninedof_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a ninedof sensor reading to complete (accelerometer, magnetometer, or gyroscope).
returncode_t libtocksync_ninedof_yield_wait_for(int* x, int* y, int* z);

#ifdef __cplusplus
}
#endif
