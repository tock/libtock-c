#pragma once

#include "../tock.h"
#include "syscalls/touch_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Touch status.
typedef enum {
  LIBTOCK_TOUCH_STATUS_RELEASED  = 0,
  LIBTOCK_TOUCH_STATUS_PRESSED   = 1,
  LIBTOCK_TOUCH_STATUS_MOVED     = 2,
  LIBTOCK_TOUCH_STATUS_UNSTARTED = 3,
} libtock_touch_status_t;

// Gesture types.
typedef enum {
  LIBTOCK_TOUCH_GESTURE_NO          = 0,
  LIBTOCK_TOUCH_GESTURE_SWIPE_UP    = 1,
  LIBTOCK_TOUCH_GESTURE_SWIPE_DOWN  = 2,
  LIBTOCK_TOUCH_GESTURE_SWIPE_LEFT  = 3,
  LIBTOCK_TOUCH_GESTURE_SWIPE_RIGHT = 4,
  LIBTOCK_TOUCH_GESTURE_ZOOM_IN     = 5,
  LIBTOCK_TOUCH_GESTURE_ZOOM_OUT    = 6,
} libtock_touch_gesture_t;

// Function signature for touch data callback.
//
// - `arg1` (`libtock_touch_status_t`): Status from touch device.
// - `arg2` (`uint16_t`): X coordinate of the touch event.
// - `arg3` (`uint16_t`): Y coordinate of the touch event.
typedef void (*libtock_touch_touch_callback)(libtock_touch_status_t, uint16_t, uint16_t);

// Function signature for multi touch data callback.
//
// - `arg1` (`returncode_t`): Status from touch device.
// - `arg2` (`int`): Number of touch events.
// - `arg3` (`int`): Number of dropped events.
// - `arg4` (`int`): Number of events that did not fit in the buffer.
typedef void (*libtock_touch_multi_touch_callback)(returncode_t, int, int, int);

// Function signature for gesture callback.
//
// - `arg1` (`returncode_t`): Status from touch device.
// - `arg2` (`libtock_touch_gesture_t`): Gesture type.
typedef void (*libtock_touch_gesture_callback)(returncode_t, libtock_touch_gesture_t);


typedef struct __attribute__((__packed__)) {
  unsigned char id;
  unsigned char status;
  unsigned short x;
  unsigned short y;
  unsigned char size;
  unsigned char pressure;
} libtock_touch_event_t;

// buffer data format
//  0         1             2                  4                  6           7               8         ...
// +---------+-------------+------------------+------------------+-----------+---------------+--------- ...
// | id (u8) | status (u8) | x (u16)          | y (u16)          | size (u8) | pressure (u8) |          ...
// +---------+-------------+------------------+------------------+-----------+---------------+--------- ...
// | Touch 0                                                                                 | Touch 1  ...



returncode_t libtock_touch_get_number_of_touches(int* touches);

returncode_t libtock_touch_enable_single_touch(libtock_touch_touch_callback cb);

returncode_t libtock_touch_disable_single_touch(void);

returncode_t libtock_touch_enable_multi_touch(libtock_touch_event_t* buffer, int max_touches,
                                              libtock_touch_multi_touch_callback cb);

returncode_t libtock_touch_disable_multi_touch(void);

returncode_t libtock_touch_allocate_multi_touch_buffer(int max_touches, libtock_touch_event_t** buffer);

returncode_t libtock_touch_get_gestures(libtock_touch_gesture_callback cb);

// Every multi touch event needs to be acked
returncode_t libtock_touch_multi_touch_next(void);

returncode_t libtock_touch_read_touch_from_buffer(libtock_touch_event_t* buffer, int index,
                                                  uint8_t* id, uint8_t* status, uint8_t* x, uint8_t* y,
                                                  uint8_t* size, uint8_t* pressure);

#ifdef __cplusplus
}
#endif
