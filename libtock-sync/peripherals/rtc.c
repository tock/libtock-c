#include <libtock/peripherals/syscalls/rtc_syscalls.h>

#include "rtc.h"

struct rtc_date_data {
  bool fired;
  returncode_t ret;
  libtock_rtc_date_t date;
};
struct rtc_done_data {
  bool fired;
  returncode_t ret;
};

static struct rtc_date_data result      = { .fired = false };
static struct rtc_done_data result_done = { .fired = false };


static void rtc_date_cb(returncode_t ret, libtock_rtc_date_t date) {
  result.fired = true;
  result.ret   = ret;
  result.date  = date;
}

static void rtc_done_cb(returncode_t ret) {
  result_done.fired = true;
  result_done.ret   = ret;
}

bool libtocksync_rtc_exists(void) {
  return libtock_rtc_driver_exists();
}

returncode_t libtocksync_rtc_get_date(libtock_rtc_date_t* date) {
  returncode_t ret;

  result.fired = false;

  ret = libtock_rtc_get_date(rtc_date_cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  *date = result.date;
  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_rtc_set_date(libtock_rtc_date_t* set_date) {
  returncode_t ret;

  result_done.fired = false;

  ret = libtock_rtc_set_date(set_date, rtc_done_cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  yield_for(&result_done.fired);

  return result_done.ret;
}
