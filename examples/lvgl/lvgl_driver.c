#include "lvgl_driver.h"

static lv_disp_buf_t disp_buf;
lv_disp_drv_t disp_drv;

/* screen driver */
static void screen_lvgl_driver(lv_disp_drv_t * disp, const lv_area_t * area,
                               __attribute__ ((unused)) lv_color_t * color_p)
{
  int32_t x, y;
  x = area->x1;
  y = area->y1;
  int w = area->x2 - area->x1 + 1;
  int h = area->y2 - area->y1 + 1;
  screen_set_frame (x, y, w, h);
  screen_write ((w * h) * sizeof(lv_color_t));

  lv_disp_flush_ready(disp);           /* Indicate you are ready with the flushing*/
}

int lvgl_driver_init (int buffer_lines)
{
  size_t width, height;
  int error = screen_get_resolution (&width, &height);
  if (error == RETURNCODE_SUCCESS) {
    error = screen_init (width * buffer_lines * sizeof(lv_color_t));
    if (error == RETURNCODE_SUCCESS) {
      /* share the frame buffer with littlevgl */
      lv_color_t *buf = (lv_color_t*)screen_buffer ();

      /* initialize littlevgl */
      lv_init ();
      lv_disp_drv_init(&disp_drv);
      disp_drv.flush_cb = screen_lvgl_driver;
      disp_drv.buffer   = &disp_buf;
      disp_drv.hor_res  = width;
      disp_drv.ver_res  = height;
      lv_disp_buf_init(&disp_buf, buf, NULL, width * buffer_lines);
      disp_drv.buffer = &disp_buf;
      lv_disp_drv_register(&disp_drv);
    }
  }
  return error;
}

void lvgl_driver_event (int millis) {
  lv_tick_inc(millis);
  lv_task_handler();
}
