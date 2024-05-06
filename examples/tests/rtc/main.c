#include <stdio.h>

#include <libtock-sync/peripherals/rtc.h>
#include <libtock-sync/services/alarm.h>

int main(void){
  // Initialises a date struct with a certain timestamp
  libtock_rtc_date_t date = {
    .year  = 2023,
    .month = JANUARY,
    .day   = 1,

    .day_of_week = MONDAY,
    .hour        = 12,
    .minute      = 30,
    .seconds     = 1
  };

  libtocksync_rtc_set_date(&date);
  // Clock has a small delay before starting to count seconds
  libtocksync_alarm_delay_ms(2000);

  while (1) {
    libtocksync_rtc_get_date(&date);
    printf("Date: {year: %d, month: %d, day: %d, day_of_week: %d, hour: %d, minute: %d, seconds: %d}\n\n", date.year,
           date.month, date.day, date.day_of_week, date.hour, date.minute, date.seconds);

    // This delay uses an underlying timer in the kernel
    libtocksync_alarm_delay_ms(1000);
  }
  return 0;
}
