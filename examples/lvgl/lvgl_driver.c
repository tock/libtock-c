#include <lvgl/lvgl.h>

#include <libtock-sync/display/screen.h>
#include <libtock/sensors/touch.h>
#include <libtock/services/alarm.h>
#include <libtock/tock.h>

#include "lvgl_driver.h"

#define PIXEL_SIZE 2 // 16 bit for RGB565

static lv_display_t* display_device;

static lv_indev_t* indev;

static int touch_status = LIBTOCK_TOUCH_STATUS_UNSTARTED;
static uint16_t touch_x = 0, touch_y = 0;

/* screen driver */
static void screen_lvgl_driver(lv_display_t* disp, const lv_area_t* area,
                               uint8_t* buffer) {
  void* ud = lv_display_get_user_data(disp);
  uint32_t buffer_size = (uint32_t)ud;
  int32_t x, y;
  x = area->x1;
  y = area->y1;
  int w = area->x2 - area->x1 + 1;
  int h = area->y2 - area->y1 + 1;
  libtocksync_screen_set_frame(x, y, w, h);
  libtocksync_screen_write(buffer, buffer_size, (w * h) * PIXEL_SIZE);

  lv_display_flush_ready(disp);           /* Indicate you are ready with the flushing*/
}

static void touch_event(libtock_touch_status_t status, uint16_t x, uint16_t y) {
  touch_status = status;
  touch_x      = x;
  touch_y      = y;
}

static void indev_cb(__attribute__ ((unused)) lv_indev_t* _indev, lv_indev_data_t* data) {
  if (touch_status == LIBTOCK_TOUCH_STATUS_PRESSED || touch_status == LIBTOCK_TOUCH_STATUS_MOVED) {
    data->point.x = touch_x;
    data->point.y = touch_y;
    data->state   = LV_INDEV_STATE_PRESSED;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

static uint32_t tick_cb(void) {
  uint32_t ticks;
  libtock_alarm_command_read(&ticks);

  uint32_t ms = libtock_alarm_ticks_to_ms(ticks);
  return ms;
}

int lvgl_driver_init(int buffer_lines) {
  uint32_t width, height;
  int error = libtock_screen_get_resolution(&width, &height);
  if (error != RETURNCODE_SUCCESS) return error;

  uint32_t buffer_size = width * buffer_lines * PIXEL_SIZE;
  uint8_t* buffer      = NULL;
  error = libtock_screen_buffer_init(buffer_size, &buffer);
  if (error != RETURNCODE_SUCCESS) return error;

  /* initialize littlevgl */
  lv_init();

  display_device = lv_display_create(width, height);
  lv_display_set_color_format(display_device, LV_COLOR_FORMAT_RGB565);
  lv_display_set_flush_cb(display_device, screen_lvgl_driver);
  lv_display_set_antialiasing(display_device, false);

  lv_display_set_buffers(display_device, buffer, NULL, buffer_size, LV_DISPLAY_RENDER_MODE_PARTIAL);
  lv_display_set_user_data(display_device, (void*)buffer_size);

  lv_tick_set_cb(tick_cb);

  int touches;
  if (libtock_touch_get_number_of_touches(&touches) == RETURNCODE_SUCCESS && touches >= 1) {
    libtock_touch_enable_single_touch(touch_event);
    indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, indev_cb);
  }

  return RETURNCODE_SUCCESS;
}
