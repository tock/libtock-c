#include "rtc.h"
#include <stdio.h>
#include <stdlib.h>

int date, time;

static void rtc_cb(int status __attribute__ ((unused)), 
                   int upcall1, 
                   int upcall2, 
                   void* ud __attribute__ ((unused))){
  date = upcall1;
  time = upcall2;
}

int get_date(struct Date *put_date){
  subscribe_return_t sub = subscribe(DRIVER_NUM_RTC, 0, rtc_cb, malloc(sizeof(struct DateTime)));
  if (!sub.success) {
    return tock_status_to_returncode(sub.status);
  }

  syscall_return_t rval = command(DRIVER_NUM_RTC, 1, 0, 0);
  if(!(rval.type == 128)){
    printf("%d", rval.type);
  }

  struct Date date_result = {
    .year = date % (1 << 21) / (1 << 9),
    .month = date % (1 << 9) / (1 << 5),
    .day = date % (1 << 5),

    .day_of_week = time % (1 << 20) / (1 << 17),
    .hour = time % (1 << 17) / (1 << 12), 
    .minute = time % (1 << 12) / (1 << 6),
    .seconds = time % (1 << 6)
  };

  *put_date = date_result;
  return tock_command_return_novalue_to_returncode(rval);
}

int set_date(const struct Date *set_date){
  date = set_date->year * (1 << 9) + set_date->month * (1 << 5) + set_date->day;
  time = set_date->day_of_week * (1 << 17) + set_date->hour * (1 << 12) + set_date->minute * (1 << 6) + set_date->seconds;

  syscall_return_t rval = command(DRIVER_NUM_RTC, 2, date, time);
  return tock_command_return_novalue_to_returncode(rval);
}
