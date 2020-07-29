#include "touch.h"

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

bool is_single_touch (void) {
    return (touch_command(100, 0, 0) > 0);
}

bool is_multi_touch (void) {
    return touch_command(101, 0, 0) > 0;
}

bool supports_gestures (void) {
    return touch_command(102, 0, 0) > 0;
}

int enable (void) {
  return touch_command(1, 0, 0);
}
int disable (void) {
  return touch_command(2, 0, 0);
}

int single_touch_set_callback (single_touch_callback cb, void* ud) {
  return touch_subscribe (0, cb, ud);
}

int multi_touch_set_callback (multi_touch_callback cb, void* ud, int max_touches) {
  int err = TOCK_SUCCESS;
  if (cb != NULL)
  {
    if (multi_touch_buffer == NULL) {
      multi_touch_buffer = (unsigned char*)malloc (max_touches * MULTI_TOUCH_LENGTH);
      if (multi_touch_buffer) 
      {
        err = touch_subscribe (1, cb, ud);
        if (err != TOCK_SUCCESS) {
          free (multi_touch_buffer);
          multi_touch_buffer = NULL;
        }
      }
      else
      {
        err = TOCK_ENOMEM;
      }
    }
    else 
    {
      err = TOCK_ALREADY;
    }
  }
  else
  {
    
    if (err == TOCK_SUCCESS) {
      multi_touch_subscriptions = multi_touch_subscriptions + 1;
    }
  } 
  return err;
}

int gesture_set_callback (gesture_callback cb, void* ud) {
  return touch_subscribe (2, cb, ud);
}
