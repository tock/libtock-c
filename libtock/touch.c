#include "touch.h"
#include <stdlib.h>

#define MULTI_TOUCH_LENGTH 8

static int touch_subscribe(int subscribe_num, subscribe_cb callback, void* callback_args) {
  return subscribe(DRIVER_NUM_TOUCH, subscribe_num, callback, callback_args);
}

static int touch_command(int command_num, int arg1, int arg2) {
  return command(DRIVER_NUM_TOUCH, command_num, arg1, arg2);
}

static int touch_allow(int allow_num, void* data, int len) {
  return allow(DRIVER_NUM_TOUCH, allow_num, data, len);
}

static unsigned char *multi_touch_buffer = NULL;
static unsigned char num_touches         = 0;

static touch_callback *single_touch_cb = NULL;
static gesture_callback *gesture_cb    = NULL;

static void touch_single_touch_callback (int status, int xy, int data2 __attribute__((unused)), void *ud) {
  if (single_touch_cb) single_touch_cb (status, ((unsigned int)xy >> 16), (unsigned int)xy & 0xFFFF, ud);
}

static void touch_gesture_callback (int gesture, int data1 __attribute__((unused)), int data2 __attribute__(
                                      (unused)), void *ud) {
  if (gesture_cb) gesture_cb (gesture, ud);
}

int get_number_of_touches (void) {
  return touch_command(100, 0, 0);
}

int single_touch_set_callback (touch_callback cb, void* ud) {
  single_touch_cb = cb;
  return touch_subscribe (0, cb != NULL ? touch_single_touch_callback : NULL, ud);
}

int multi_touch_set_callback (touch_callback cb, void* ud, int max_touches) {
  int err = TOCK_SUCCESS;
  if (cb != NULL) {
    if (multi_touch_buffer == NULL) {
      multi_touch_buffer = (unsigned char*)malloc (max_touches * MULTI_TOUCH_LENGTH);
      if (multi_touch_buffer) {
        num_touches = max_touches;
        err         = touch_allow (2, multi_touch_buffer, max_touches * MULTI_TOUCH_LENGTH);
        if (err == TOCK_SUCCESS) err = touch_subscribe (2, cb, ud);
        if (err != TOCK_SUCCESS) {
          free (multi_touch_buffer);
          multi_touch_buffer = NULL;
        }
      }else {
        err = TOCK_ENOMEM;
      }
    }else {
      err = TOCK_EALREADY;
    }
  }else {
    if (multi_touch_buffer != NULL) {
      num_touches = 0;
      touch_allow (2, NULL, 0);
      err = touch_subscribe (2, cb, ud);
      free (multi_touch_buffer);
      multi_touch_buffer = NULL;
    }
  }
  return err;
}

int gesture_set_callback (gesture_callback cb, void* ud) {
  gesture_cb = cb;
  return touch_subscribe (1, cb != NULL ? touch_gesture_callback : NULL, ud);
}

// get multi touch

int read_touch (int index, unsigned char *id, unsigned char *type, unsigned short *x, unsigned short *y) {
  if (index < num_touches) {
    if (multi_touch_buffer != NULL) {
      *id   = multi_touch_buffer[index * 8 + 0];
      *type = multi_touch_buffer[index * 8 + 1];
      *x    = (multi_touch_buffer[index * 8 + 2] << 8) + multi_touch_buffer[index * 8 + 3];
      *y    = (multi_touch_buffer[index * 8 + 4] << 8) + multi_touch_buffer[index * 8 + 5];
      return TOCK_SUCCESS;
    }else {
      return TOCK_ENOMEM;
    }
  }else {
    return TOCK_EINVAL;
  }
}

int read_touch_full (int index, unsigned char *id, unsigned char *type, unsigned short *x, unsigned short *y,
                     unsigned char *size, unsigned char *pressure) {
  if (multi_touch_buffer != NULL) {
    int err = read_touch (index, id, type, x, y);
    if (err == TOCK_SUCCESS) {
      *size     = multi_touch_buffer[index * 8 + 6];
      *pressure = multi_touch_buffer[index * 8 + 7];
    }
    return err;
  }else {
    return TOCK_ENOMEM;
  }
}