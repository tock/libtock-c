#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kv.h>
#include <unit_test.h>

#define KEY_LEN  200
#define DATA_LEN 3000

uint8_t key_buf[KEY_LEN];
uint8_t data_buf[DATA_LEN];
uint8_t value_buf[DATA_LEN];


static bool test_exists(void) {
  int ret = kv_check_status();
  CHECK(ret == RETURNCODE_SUCCESS);
  return true;
}

static bool test_set_get_no_permissions(void) {
  int ret;
  char key[] = "kvnoreadtestapp";
  strcpy((char*) key_buf, key);

  uint32_t value_len = 45;
  for (uint32_t i = 0; i < value_len; i++) {
    value_buf[i] = (uint8_t) i;
  }

  ret = kv_set_sync(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  ret = kv_get_sync(key_buf, strlen(key), data_buf, DATA_LEN, &value_len);
  CHECK(ret == RETURNCODE_ENOSUPPORT);

  return true;
}

int main(void) {
  unit_test_fun tests[] = {
    TEST(exists),
    TEST(set_get_no_permissions),
  };
  unit_test_runner(tests, sizeof(tests) / sizeof(unit_test_fun), 2000, "org.tockos.unit_test");
  return 0;
}
