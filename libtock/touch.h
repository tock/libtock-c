// Touch Panel Library

#pragma once

#include "tock.h"

#define DRIVER_NUM_TOUCH 0x90002

#define TOUCH_STATUS_RELEASED 0
#define TOUCH_STATUS_PRESSED 1

#define GESTURE_NO 0
#define GESTURE_SWIPE_UP 1
#define GESTURE_SWIPE_DOWN 2
#define GESTURE_SWIPE_LEFT 3
#define GESTURE_SWIPE_RIGHT 4
#define GESTURE_ZOOM_IN 5
#define GESTURE_ZOOM_OUT 6

typedef void (single_touch_callback)(int, int, int, void*);
typedef void (multi_touch_callback)(int, int, int, void*);
typedef void (gesture_callback)(int, void*);

bool is_single_touch (void);
bool is_multi_touch (void);
bool supports_gestures (void);

int enable (void);
int disable (void);

int single_touch_set_callback (single_touch_callback cb, void* ud);
int multi_touch_set_callback (multi_touch_callback cb, void* ud, int max_touches);
int gesture_set_callback (gesture_callback cb, void* ud);
