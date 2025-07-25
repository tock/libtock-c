#include "rtc.h"

// DateTime codifies Date structure into two u32 (int) numbers
//     date: first number (year, month, day_of_the_month):
//            - last 5 bits store the day_of_the_month
//            - previous 4 bits store the month
//            - previous 12 bits store the year
//     time: second number (day_of_the_week, hour, minute, seconds):
//            - last 6 bits store the seconds
//            - previous 6 store the minute
//            - previous 5 store the hour
//            - previous 3 store the day_of_the_week
static void rtc_convert_args_to_date(uint32_t date, uint32_t time, libtock_rtc_date_t* out) {
  out->year  = date % (1 << 21) / (1 << 9);
  out->month = date % (1 << 9) / (1 << 5);
  out->day   = date % (1 << 5);

  out->day_of_week = time % (1 << 20) / (1 << 17);
  out->hour        = time % (1 << 17) / (1 << 12);
  out->minute      = time % (1 << 12) / (1 << 6);
  out->seconds     = time % (1 << 6);
}

static void rtc_date_cb(int   status,
                        int   date,
                        int   time,
                        void* opaque) {
  libtock_rtc_callback_date cb = (libtock_rtc_callback_date) opaque;
  libtock_rtc_date_t rtc_date;

  rtc_convert_args_to_date((uint32_t) date, (uint32_t) time, &rtc_date);
  cb(tock_status_to_returncode(status), rtc_date);
}

static void rtc_set_cb(int                          status,
                       __attribute__ ((unused)) int arg1,
                       __attribute__ ((unused)) int arg2,
                       void*                        opaque) {
  libtock_rtc_callback_done cb = (libtock_rtc_callback_done) opaque;
  cb(tock_status_to_returncode(status));
}

returncode_t libtock_rtc_get_date(libtock_rtc_callback_date cb) {
  returncode_t ret;

  ret = libtock_rtc_set_upcall(rtc_date_cb, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_rtc_command_get_date();
  return ret;
}

returncode_t libtock_rtc_set_date(libtock_rtc_date_t* set_date, libtock_rtc_callback_done cb) {
  uint32_t date = set_date->year * (1 << 9) + set_date->month * (1 << 5) + set_date->day;
  uint32_t time = set_date->day_of_week *
                  (1 << 17) + set_date->hour * (1 << 12) + set_date->minute * (1 << 6) + set_date->seconds;

  returncode_t ret;

  ret = libtock_rtc_set_upcall(rtc_set_cb, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_rtc_command_set_date(date, time);
  return ret;
}
