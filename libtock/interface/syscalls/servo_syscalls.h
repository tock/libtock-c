#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_SERVO 0x90009

// Check if the servo system call driver is available on this board.
bool libtock_servo_exists(void);

// Change the angle.
returncode_t libtock_servo_angle(uint32_t angle);
// Requests the current angle from the servo.
returncode_t libtock_current_servo_angle(uint16_t* angle);

#ifdef __cplusplus
}
#endif
