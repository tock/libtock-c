#pragma once

#include "tock.h"
#include "syscalls/rtc_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

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
typedef struct {
  int year;
  int month;
  int day;
  int day_of_week;
  int hour;
  int minute;
  int seconds;
} libtock_rtc_date_t;

// Function signature for get date callbacks.
//
// - `arg1` (`returncode_t`): Returncode indicating status.
// - `arg2` (`libtock_rtc_date_t`): Current date.
typedef void (*libtock_rtc_callback_date)(returncode_t, libtock_rtc_date_t);

// Function signature for operation done callbacks.
//
// - `arg1` (`returncode_t`): Returncode indicating status.
typedef void (*libtock_rtc_callback_done)(returncode_t);


// Get the current date.
//
// The callback will be issued with the current date and time.
returncode_t libtock_rtc_get_date(libtock_rtc_callback_date cb);

// Set the date and time.
//
// The callback will be issued when the time and date have been set.
returncode_t libtock_rtc_set_date(libtock_rtc_date_t* set_date, libtock_rtc_callback_done cb);

#ifdef __cplusplus
}
#endif
