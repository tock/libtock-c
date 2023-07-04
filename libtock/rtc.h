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

struct Date{
  int year;
  int month;
  int day;
  int day_of_week;
  int hour;
  int minute;
  int seconds;
};

int get_date(struct Date *get_date);
int set_date(struct Date set_date);

#ifdef __cplusplus
}
#endif
