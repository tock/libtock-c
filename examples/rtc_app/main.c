#include <stdio.h>
#include <rtc.h>
#include <timer.h>

int main(void){
  // Initialises a date struct with a certain timestamp
  struct Date date = {
    .year = 2023,
    .month = JANUARY,
    .day = 1,
    
    .day_of_week = MONDAY,
    .hour = 12,
    .minute = 30,
    .seconds = 1
  };

  set_date(date);
  printf("Date: {year: %d, month: %d, day: %d, day_of_week: %d, hour: %d, minute: %d, seconds: %d}\n", date.year, date.month, date.day, date.day_of_week, date.hour, date.minute, date.seconds);

  while(1){
    get_date(&date);
    printf("Date: {year: %d, month: %d, day: %d, day_of_week: %d, hour: %d, minute: %d, seconds: %d}\n", date.year, date.month, date.day, date.day_of_week, date.hour, date.minute, date.seconds);

	  // This delay uses an underlying timer in the kernel
	  delay_ms(1000);
  }
  return 0;
}

