#include <stdbool.h>
#include <stdio.h>

#include <libtock-sync/services/unit_test.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/tock.h>



static bool test_pass(void) {
  return true;
}

static bool test_fail(void) {
  return false;
}


int main(void) {
  unit_test_fun tests[6] = { TEST(pass), TEST(pass), TEST(pass), TEST(fail), TEST(fail), TEST(pass) };
  unit_test_runner(tests, 6, 300, "org.tockos.unit_test");

  while (1) {
    yield();
  }
}
