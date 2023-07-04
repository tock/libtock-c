#include "rtc.h"
#include <stdio.h>

int get_date(struct Date *get_date){
  syscall_return_t rval = command(DRIVER_NUM_RTC, 1, 0, 0);
  int date = rval.data[1];
  int time = rval.data[2];

  struct Date date_result = {
    .year = date % (1 << 21) / (1 << 9),
    .month = date % (1 << 9) / (1 << 5),
    .day = date % (1 << 5),

    .day_of_week = time % (1 << 20) / (1 << 17),
    .hour = time % (1 << 17) / (1 << 12), 
    .minute = time % (1 << 12) / (1 << 6),
    .seconds = time % (1 << 6)
  };

  printf("%d - %d\n", date, time);

  *get_date = date_result;
  return tock_command_return_novalue_to_returncode(rval);
}

int set_date(struct Date set_date){
  printf("Setting Date\n");
  int date = set_date.year * (1 << 9) + set_date.month * (1 << 5) + set_date.day;
  int time = set_date.day_of_week * (1 << 17) + set_date.hour * (1 << 12) + set_date.minute * (1 << 6) + set_date.seconds;

  syscall_return_t rval = command(DRIVER_NUM_RTC, 2, date, time);
  return tock_command_return_novalue_to_returncode(rval);
}
