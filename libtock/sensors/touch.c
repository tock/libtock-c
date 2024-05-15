#include <stdlib.h>

#include "touch.h"

static void single_touch_upcall(int                          status,
                                int                          xy,
                                __attribute__ ((unused)) int unused1,
                                void*                        opaque) {
  libtock_touch_touch_callback cb = (libtock_touch_touch_callback) opaque;

  uint16_t x = (uint16_t) (((uint32_t) xy) >> 16);
  uint16_t y = (uint16_t) (((uint32_t) xy) & 0xFFFF);

  cb(status, x, y);
}

static void multi_touch_upcall(int   num_events,
                               int   dropped_events,
                               int   lost_touches,
                               void* opaque) {
  libtock_touch_multi_touch_callback cb = (libtock_touch_multi_touch_callback) opaque;

  cb(RETURNCODE_SUCCESS, num_events, dropped_events, lost_touches);
}

static void gesture_upcall(int                          gesture,
                           __attribute__ ((unused)) int unused0,
                           __attribute__ ((unused)) int unused1,
                           void*                        opaque) {
  libtock_touch_gesture_callback cb = (libtock_touch_gesture_callback) opaque;

  cb(RETURNCODE_SUCCESS, gesture);
}

returncode_t libtock_touch_get_number_of_touches(int* touches) {
  return libtock_touch_command_get_number_of_touches((uint32_t*) touches);
}

returncode_t libtock_touch_enable_single_touch(libtock_touch_touch_callback cb) {
  returncode_t ret;

  ret = libtock_touch_set_upcall_single_touch(single_touch_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_touch_command_enable_single_touch();
  return ret;
}

returncode_t libtock_touch_disable_single_touch(void) {
  returncode_t ret;

  ret = libtock_touch_set_upcall_single_touch(NULL, NULL);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_touch_command_disable_single_touch();
  return ret;
}

returncode_t libtock_touch_enable_multi_touch(libtock_touch_event_t* buffer, int max_touches,
                                              libtock_touch_multi_touch_callback cb) {
  returncode_t ret;

  ret = libtock_touch_set_allow_readwrite_multi_touch(buffer, max_touches * sizeof(libtock_touch_event_t));
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_touch_set_upcall_multi_touch(multi_touch_upcall, cb);
  return ret;

}

returncode_t libtock_touch_disable_multi_touch(void) {
  returncode_t ret;

  ret = libtock_touch_set_allow_readwrite_multi_touch(NULL, 0);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_touch_set_upcall_multi_touch(NULL, NULL);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_touch_command_disable_multi_touch();
  return ret;
}

returncode_t libtock_touch_allocate_multi_touch_buffer(int max_touches, libtock_touch_event_t** buffer) {
  libtock_touch_event_t* multi_touch_buffer;
  multi_touch_buffer = (libtock_touch_event_t*) malloc(max_touches * sizeof(libtock_touch_event_t));

  if (multi_touch_buffer == NULL) {
    return RETURNCODE_ENOMEM;
  } else {
    *buffer = multi_touch_buffer;
    return RETURNCODE_SUCCESS;
  }
}

returncode_t libtock_touch_get_gestures(libtock_touch_gesture_callback cb) {
  return libtock_touch_set_upcall_gesture(gesture_upcall, cb);
}

returncode_t libtock_touch_multi_touch_next(void) {
  return libtock_touch_command_multi_touch_next();
}


// get multi touch

returncode_t libtock_touch_read_touch_from_buffer(libtock_touch_event_t* buffer, int index,
                                                  uint8_t* id, uint8_t* status, uint8_t* x, uint8_t* y,
                                                  uint8_t* size, uint8_t* pressure) {
  if (buffer != NULL) {
    *id       = buffer[index].id;
    *status   = buffer[index].status;
    *x        = buffer[index].x;
    *y        = buffer[index].y;
    *size     = buffer[index].size;
    *pressure = buffer[index].pressure;
    return RETURNCODE_SUCCESS;
  } else {
    return RETURNCODE_ENOMEM;
  }
}
