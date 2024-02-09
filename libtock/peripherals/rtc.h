#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_RTC 0x00090007

#define JANUARY 1
#define FEBRUARY 2
#define MARCH 3
#define APRIL 4
#define MAY 5
#define JUNE 6
#define JULY 7
#define AUGUST 8
#define SEPTEMBER 9
#define OCTOBER 10
#define NOVEMBER 11
#define DECEMBER 12

#define SUNDAY 0
#define MONDAY 1
#define TUESDAY 2
#define WENSDAY 3
#define THURSDAY 4
#define FRIDAY 5
#define SATURDAY 6

// Date structure to store date and time
struct Date{
  int year;
  int month;
  int day;
  int day_of_week;
  int hour;
  int minute;
  int seconds;
};

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
struct DateTime{
  int date;
  int time;
};

// Fetches current date from registers and writes it to get_date
int get_date(struct Date *get_date);

// Writes set_date into the registers
int set_date(const struct Date *set_date);

#ifdef __cplusplus
}
#endif
