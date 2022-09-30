#include "touch.h"
#include <stdlib.h>

static int touch_subscribe(int subscribe_num, subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_TOUCH, subscribe_num, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

static int touch_allow(int allow_num, touch_t* data, int len, touch_t **prev_buffer) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_TOUCH, allow_num, data, len);
  if (prev_buffer != NULL) {
    *prev_buffer = aval.ptr;
  }
  return tock_allow_rw_return_to_returncode(aval);
}

static bool registered = false;
static touch_t *multi_touch_buffer = NULL;

static unsigned char num_registered_touches = 0;

static touch_single_touch_callback *single_touch_upcall = NULL;
static touch_multi_touch_callback *multi_touch_upcall   = NULL;
static touch_gesture_callback *gesture_upcall = NULL;

static void single_touch_callback (int status, int xy, int data2 __attribute__((unused)), void *ud) {
  if (single_touch_upcall) single_touch_upcall(status, ((unsigned int)xy >> 16), (unsigned int)xy & 0xFFFF, ud);
}

static void multi_touch_callback (int num_touches, int dropped_events, int data2 __attribute__(
                                    (unused)), void *ud) {
  touch_allow(0, multi_touch_buffer, num_registered_touches * sizeof(touch_t), &multi_touch_buffer);
  if (multi_touch_upcall) multi_touch_upcall(num_touches, dropped_events, ud);
}

static void gesture_callback (int gesture, int data1 __attribute__((unused)), int data2 __attribute__(
                                (unused)), void *ud) {
  if (gesture_upcall) gesture_upcall(gesture, ud);
}

int get_number_of_touches (int* touches) {
  syscall_return_t cval = command(DRIVER_NUM_TOUCH, 100, 0, 0);
  return tock_command_return_u32_to_returncode(cval, (uint32_t*) touches);
}

int enable_single_touch(void) {
  syscall_return_t cval = command(DRIVER_NUM_TOUCH, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int disable_single_touch(void) {
  syscall_return_t cval = command(DRIVER_NUM_TOUCH, 2, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int single_touch_set_callback (touch_single_touch_callback cb, void* ud) {
  single_touch_upcall = cb;
  return touch_subscribe(0, cb != NULL ? single_touch_callback : NULL, ud);
}

int enable_multi_touch(void) {
  syscall_return_t cval = command(DRIVER_NUM_TOUCH, 11, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int disable_multi_touch(void) {
  syscall_return_t cval = command(DRIVER_NUM_TOUCH, 12, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int multi_touch_set_callback (touch_multi_touch_callback cb, void* ud, int max_touches) {
  int err = RETURNCODE_SUCCESS;
  if (cb != NULL) {
    if (registered == false) {
      multi_touch_buffer = (touch_t*)malloc(max_touches * sizeof(touch_t));
      if (multi_touch_buffer) {
        touch_t * shared_buffer = (touch_t*)malloc(max_touches * sizeof(touch_t));
        if (shared_buffer) {
          num_registered_touches = max_touches;
          err = touch_allow(0, shared_buffer, max_touches * sizeof(touch_t), NULL);
          if (err == RETURNCODE_SUCCESS) {
            shared_buffer      = NULL;
            multi_touch_upcall = cb;
            err = touch_subscribe(2, multi_touch_callback, ud);
          }
          if (err != RETURNCODE_SUCCESS) {
            free(multi_touch_buffer);
            multi_touch_buffer = NULL;
            touch_allow(0, NULL, 0, &shared_buffer);
            if (shared_buffer != NULL) {
              free(shared_buffer);
            }
          } else {
            registered = true;
          }
        } else {
          free(multi_touch_buffer);
          err = RETURNCODE_ENOMEM;
        }
      } else {
        err = RETURNCODE_ENOMEM;
      }
    } else {
      err = RETURNCODE_EALREADY;
    }
  } else {
    if (registered) {
      num_registered_touches = 0;
      touch_t * buffer = NULL;
      touch_allow(0, NULL, 0, &buffer);
      if (buffer != NULL) {
        free(buffer);
      }
      err = touch_subscribe(2, NULL, ud);
      free(multi_touch_buffer);
      multi_touch_buffer = NULL;
      registered         = false;
    }
  }
  return err;
}

int gesture_set_callback (touch_gesture_callback cb, void* ud) {
  gesture_upcall = cb;
  return touch_subscribe(1, cb != NULL ? gesture_callback : NULL, ud);
}

// get multi touch

int read_touch (int index, unsigned char *id, unsigned char *status, unsigned short *x, unsigned short *y) {
  if (index < num_registered_touches) {
    if (multi_touch_buffer != NULL) {
      *id     = multi_touch_buffer[index].id;
      *status = multi_touch_buffer[index].status;
      *x      = multi_touch_buffer[index].x;
      *y      = multi_touch_buffer[index].y;
      return RETURNCODE_SUCCESS;
    } else {
      return RETURNCODE_ENOMEM;
    }
  } else {
    return RETURNCODE_EINVAL;
  }
}

int read_touch_full (int index, unsigned char *id, unsigned char *status, unsigned short *x, unsigned short *y,
                     unsigned char *size, unsigned char *pressure) {
  if (multi_touch_buffer != NULL) {
    int err = read_touch(index, id, status, x, y);
    if (err == RETURNCODE_SUCCESS) {
      *size     = multi_touch_buffer[index].size;
      *pressure = multi_touch_buffer[index].pressure;
    }
    return err;
  } else {
    return RETURNCODE_ENOMEM;
  }
}
