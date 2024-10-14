#include <stdio.h>

#include <libtock-sync/display/screen.h>
#include <libtock-sync/peripherals/rtc.h>
#include <libtock-sync/services/alarm.h>

#include <lvgl/lvgl.h>

#include "lvgl_driver.h"

libtock_rtc_date_t date;
uint32_t reference = 0;
lv_obj_t* label1;
lv_obj_t* seconds_lbl;
lv_obj_t* batt_lbl;
libtock_alarm_t rtc_alarm;
libtock_alarm_t update_alarm;
char label1_buffer[8];
char seconds_buffer[4];
char batt_buffer[8];

void rtc_callback(uint32_t now, uint32_t scheduled, void* ud);

void rtc_done(returncode_t success, libtock_rtc_date_t new_date) {
  date = new_date;
  libtock_alarm_command_read(&reference);
  date.hour += 24 - 7;
  date.hour = date.hour % 24;
  libtock_alarm_in_ms(60000, rtc_callback, &date, &rtc_alarm);
}

void rtc_callback(uint32_t now, uint32_t scheduled, void* ud) {
  libtock_rtc_get_date(rtc_done);
}

void update_callback(uint32_t now, uint32_t scheduled, void* ud) {

  uint16_t sample;
  int err = libtocksync_adc_sample(0, &sample);
  if (err == 0) {
    sprintf(batt_buffer, "%d", sample);
  } else {
    sprintf(batt_buffer, "Err");
  }
  lv_label_set_text(batt_lbl, batt_buffer);

  printf("%u %u\n", now, reference);

  uint32_t diff_seconds = libtock_alarm_ticks_to_ms(now - reference) / 1000;

  int seconds = date.seconds + diff_seconds;
  int minutes = date.minute + seconds / 60;
  int hours = date.hour + minutes / 60;

  seconds = seconds % 60;
  minutes = minutes % 60;
  hours = hours % 24;

  const char *ampm = "am";
  if (date.hour > 12) {
    ampm = "pm";
  }
  hours = hours % 12;

  sprintf(label1_buffer, "%02d:%02d%s", hours, minutes, ampm);
  lv_label_set_text(label1, label1_buffer);
  sprintf(seconds_buffer, "%02d", seconds);
  lv_label_set_text(seconds_lbl, seconds_buffer);

  lv_task_handler();
  lv_tick_inc(LV_DEF_REFR_PERIOD);
  libtock_alarm_in_ms(1000, update_callback, &date, &update_alarm);
}

int main(void) {
  libtocksync_screen_set_brightness(100);
  int status = lvgl_driver_init(5);
  if (status == RETURNCODE_SUCCESS) {
    /* LittlevGL's Hello World tutorial example */

    lv_obj_t* scr = lv_screen_active();          /*Get the current screen*/

    lv_obj_t * obj1 = lv_obj_create(lv_screen_active());
    lv_obj_set_height(obj1, lv_pct(100));
    lv_obj_set_width(obj1, lv_pct(100));

    batt_lbl = lv_label_create(scr);
    lv_label_set_text(batt_lbl, "???");
    lv_obj_align(batt_lbl, LV_ALIGN_TOP_LEFT, 0, 0);

    label1 = lv_label_create(scr);
    lv_label_set_text(label1, "00:00am");
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);

    seconds_lbl = lv_label_create(scr);
    lv_label_set_text(seconds_lbl, "00");
    lv_obj_align(seconds_lbl, LV_ALIGN_CENTER, 0, 40);

    lv_task_handler();

    libtock_alarm_in_ms(1, rtc_callback, &date, &rtc_alarm);
    libtock_alarm_in_ms(1, update_callback, &date, &update_alarm);

    while (1) {
      yield();
    }

  } else {
    printf("lvgl init error: %s\n", tock_strrcode(status));
  }
  return 0;
}
