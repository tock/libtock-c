// Touch Panel Library

#pragma once

#include "tock.h"

#define DRIVER_NUM_TOUCH 0x90002

#define TOUCH_STATUS_RELEASED 0
#define TOUCH_STATUS_PRESSED 1
#define TOUCH_STATUS_MOVED 2

#define GESTURE_NO 0
#define GESTURE_SWIPE_UP 1
#define GESTURE_SWIPE_DOWN 2
#define GESTURE_SWIPE_LEFT 3
#define GESTURE_SWIPE_RIGHT 4
#define GESTURE_ZOOM_IN 5
#define GESTURE_ZOOM_OUT 6

typedef void (touch_callback)(int, int, int, void*);
typedef void (gesture_callback)(int, void*);

// buffer data format
//  0         1           2                  4                  6           7             8         ...
// +---------+-----------+------------------+------------------+-----------+---------------+--------- ...
// | id (u8) | type (u8) | x (u16)          | y (u16)          | size (u8) | pressure (u8) |          ...
// +---------+-----------+------------------+------------------+-----------+---------------+--------- ...
// | Touch 0                                                                               | Touch 1  ...

int get_number_of_touches (void);

int single_touch_set_callback (touch_callback cb, void* ud);
int multi_touch_set_callback (touch_callback cb, void* ud, int max_touches);
int gesture_set_callback (gesture_callback cb, void* ud);

int read_touch (int index, unsigned char *id, unsigned char *type, unsigned short *x, unsigned short *y);
int read_touch_full (int index, unsigned char *id, unsigned char *type, unsigned short *x, unsigned short *y, unsigned char *size, unsigned char *pressure);
