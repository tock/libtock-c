#include <stdio.h>
#include <libtock-sync/services/alarm.h>
#include <lvgl/lvgl.h>
#include <lvgl/demos/lv_demos.h>
#include "lvgl_driver.h"

lv_obj_t* batt_lbl;

lv_obj_t* scr;

int main(void) {
  libtocksync_screen_set_brightness(100);
  int status = lvgl_driver_init(50);
  if (status == RETURNCODE_SUCCESS) {
    lv_demo_scroll();

    for ( ;;) {
      uint32_t time_till_next = lv_timer_handler();
      libtocksync_alarm_delay_ms(time_till_next);
    }

  } else {
    printf("lvgl init error: %s\n", tock_strrcode(status));
  }
  return 0;
}
