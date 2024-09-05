#include <stdio.h>

#include <libtock-sync/display/screen.h>
#include <libtock-sync/services/alarm.h>

#include <lvgl/lvgl.h>

#include "lvgl_driver.h"

/*static void event_handler(lv_event_t* e) {
  lv_event_code_t code  = lv_event_get_code(e);
  unsigned int* seconds = (unsigned int*)lv_event_get_user_data(e);

  if (code == LV_EVENT_CLICKED) {
    LV_LOG_USER("Clicked");
    *seconds = 0;
  } else if (code == LV_EVENT_VALUE_CHANGED) {
    LV_LOG_USER("Toggled");
  }
}*/

int main(void) {
  libtocksync_screen_set_brightness(100);
  int status = lvgl_driver_init(5);
  if (status == RETURNCODE_SUCCESS) {
    /* LittlevGL's Hello World tutorial example */

    lv_obj_t* scr = lv_screen_active();          /*Get the current screen*/

    lv_obj_t * obj1 = lv_obj_create(lv_screen_active());
    lv_obj_set_height(obj1, lv_pct(100));
    lv_obj_set_width(obj1, lv_pct(100));

    lv_obj_t* label1 = lv_label_create(scr);

    lv_label_set_text(label1, "Hello world!");

    lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t* btn1 = lv_btn_create(scr);
    //lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, &seconds);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);

    lv_obj_t* label = lv_label_create(btn1);
    lv_label_set_text(label, "Reset");
    lv_obj_center(label);

    lv_task_handler();

    int seconds = 0;
    while (1) {
      libtocksync_alarm_delay_ms(1000);
      seconds++;
      char buffer[100];
      int written = snprintf(buffer, sizeof(buffer), "%d", seconds);
      lv_label_set_text(label1, buffer);
      lv_task_handler();
      lv_tick_inc(LV_DEF_REFR_PERIOD);
    }

  } else {
    printf("lvgl init error: %s\n", tock_strrcode(status));
  }
  return 0;
}
