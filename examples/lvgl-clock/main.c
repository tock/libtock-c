#include <stdio.h>

#include <libtock-sync/display/screen.h>
#include <libtock-sync/peripherals/rtc.h>
#include <libtock-sync/peripherals/adc.h>
#include <libtock-sync/services/alarm.h>

#include <lvgl/lvgl.h>

#include "lvgl_driver.h"

libtock_rtc_date_t date;
uint32_t reference = 0;
lv_obj_t* label1;
lv_obj_t* seconds_lbl;
lv_obj_t* batt_lbl;

lv_obj_t* scr;
lv_obj_t* scr2;

static void rtc_done(returncode_t, libtock_rtc_date_t new_date) {
  date = new_date;
  libtock_alarm_command_read(&reference);
  date.hour += 24 - 7;
  date.hour = date.hour % 24;
}

static void rtc_callback(lv_timer_t*) {
  libtock_rtc_get_date(rtc_done);
}

static void update_callback(lv_timer_t* timer) {
  uint32_t now;
  libtock_rtc_date_t* ldate = lv_timer_get_user_data(timer);

  libtock_alarm_command_read(&now);

  uint16_t sample;
  int err = libtocksync_adc_sample(0, &sample);
  if (err == 0) {
    lv_label_set_text_fmt(batt_lbl, "%d", sample);
  } else {
    lv_label_set_text_fmt(batt_lbl, "Err");
  }

  uint32_t diff_seconds = libtock_alarm_ticks_to_ms(now - reference) / 1000;

  int seconds = ldate->seconds + diff_seconds;
  int minutes = ldate->minute + seconds / 60;
  int hours = ldate->hour + minutes / 60;

  seconds = seconds % 60;
  minutes = minutes % 60;
  hours = hours % 24;

  const char *ampm = "am";
  if (ldate->hour > 12) {
    ampm = "pm";
  }
  hours = hours % 12;

  lv_label_set_text_fmt(label1, "%02d:%02d%s", hours, minutes, ampm);
  lv_label_set_text_fmt(seconds_lbl, "%02d", seconds);

  /*if (seconds % 20 == 0) {
    lv_screen_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 2000, 0, false);
  } else if (seconds % 10 == 0) {
    lv_screen_load_anim(scr2, LV_SCR_LOAD_ANIM_MOVE_LEFT, 2000, 0, false);
  }*/
}

int main(void) {
  libtocksync_screen_set_brightness(100);
  int status = lvgl_driver_init(5);
  if (status == RETURNCODE_SUCCESS) {
    /* LittlevGL's Hello World tutorial example */

    scr = lv_screen_active();          /*Get the current screen*/

    scr2 = lv_obj_create(NULL);          /*Get the current screen*/
    batt_lbl = lv_label_create(scr2);
    lv_label_set_text(batt_lbl, "???");
    lv_obj_align(batt_lbl, LV_ALIGN_TOP_LEFT, 0, 0);

    label1 = lv_label_create(scr);
    lv_label_set_text(label1, "00:00am");
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);

    seconds_lbl = lv_label_create(scr);
    lv_label_set_text(seconds_lbl, "00");
    lv_obj_align(seconds_lbl, LV_ALIGN_CENTER, 0, 40);

    lv_timer_create(rtc_callback, 60000, (void*)&date);
    lv_timer_create(update_callback, 1000, (void*)&date);

    for ( ;;) {
      uint32_t time_till_next = lv_timer_handler();
      libtocksync_alarm_delay_ms(time_till_next);
    }

  } else {
    printf("lvgl init error: %d\n", status);
  }
  return 0;
}
