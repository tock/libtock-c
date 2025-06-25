#include <stdio.h>

#include <lvgl/lvgl.h>

#include <libtock-sync/display/screen.h>
#include <libtock-sync/services/alarm.h>

#include "lvgl_driver.h"

static uint32_t seconds = 0;

static void tick_cb(lv_timer_t* timer) {
  seconds++;
  lv_obj_t* label1 = lv_timer_get_user_data(timer);
  lv_label_set_text_fmt(label1, "%lu", seconds);
}

static void event_handler(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  uint32_t* secs       = (uint32_t*)lv_event_get_user_data(e);

  if (code == LV_EVENT_CLICKED) {
    LV_LOG_USER("Clicked");
    *secs = 0;
  } else if (code == LV_EVENT_VALUE_CHANGED) {
    LV_LOG_USER("Toggled");
  }
}

int main(void) {
  libtocksync_screen_set_brightness(100);
  int status = lvgl_driver_init(5);
  if (status == RETURNCODE_SUCCESS) {
    /* LittlevGL's Hello World tutorial example */

    lv_obj_t* scr = lv_screen_active();          /*Get the current screen*/

    lv_obj_t* obj1 = lv_obj_create(lv_screen_active());
    lv_obj_set_width(obj1, lv_pct(100));
    lv_obj_set_height(obj1, lv_pct(100));

    lv_obj_t* label1 = lv_label_create(scr);

    lv_label_set_text(label1, "0");

    lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t* btn1 = lv_btn_create(scr);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, &seconds);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);

    lv_obj_t* label = lv_label_create(btn1);
    lv_label_set_text(label, "Reset");
    lv_obj_center(label);

    lv_timer_create(tick_cb, 1000, (void*)label1);

    for ( ;;) {
      uint32_t time_till_next = lv_timer_handler();
      libtocksync_alarm_delay_ms(time_till_next);
    }
  } else {
    printf("lvgl init error: %s\n", tock_strrcode(status));
  }
  return 0;
}
