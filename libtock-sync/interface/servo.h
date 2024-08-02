#pragma once
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// ## Arguments
//
// - `angle`: The angle you want the servo to move.
//
// ## Return Value
//
// A returncode indicating whether the angle was changed successfully.
returncode_t libtocksync_servo_angle(uint32_t angle);

#ifdef __cplusplus
}
#endif