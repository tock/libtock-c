#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kv_system.h>
#include <unit_test.h>

#define KEY_LEN  200
#define DATA_LEN 3000

uint8_t key_buf[KEY_LEN];
uint8_t data_buf[DATA_LEN];
uint8_t value_buf[DATA_LEN];


static bool test_exists(void) {
  int ret = kv_system_check_status();
  CHECK(ret == RETURNCODE_SUCCESS);
  return true;
}

static bool test_set_get(void) {
  int ret;
  char key[] = "kvtestapp";
  strcpy((char*) key_buf, key);

  uint32_t value_len = 45;
  for (uint32_t i = 0; i < value_len; i++) {
    value_buf[i] = (uint8_t) i;
  }

  ret = kv_system_set_sync(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  ret = kv_system_get_sync(key_buf, strlen(key), data_buf, DATA_LEN, &value_len);
  CHECK(ret == RETURNCODE_SUCCESS);
  CHECK(value_len == 45);
  for (uint32_t i = 0; i < value_len; i++) {
    CHECK(value_buf[i] == (uint8_t) i);
  }

  return true;
}

static bool test_set_get_too_long(void) {
  int ret;
  char key[] = "kvtestapp";
  strcpy((char*) key_buf, key);

  uint32_t value_len = 10;
  for (uint32_t i = 0; i < value_len; i++) {
    value_buf[i] = (uint8_t) i;
  }

  ret = kv_system_set_sync(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  ret = kv_system_get_sync(key_buf, strlen(key), data_buf, 2, &value_len);
  CHECK(ret == RETURNCODE_ESIZE);
  CHECK(value_len == 10);
  for (uint32_t i = 0; i < 2; i++) {
    CHECK(value_buf[i] == (uint8_t) i);
  }

  return true;
}

static bool test_key_too_long(void) {
  int ret;

  uint32_t value_len = 0;
  ret = kv_system_get_sync(key_buf, KEY_LEN, value_buf, 50, &value_len);
  CHECK(ret == RETURNCODE_ESIZE);

  value_buf[0] = 'V';
  ret = kv_system_set_sync(key_buf, KEY_LEN, value_buf, 1);
  CHECK(ret == RETURNCODE_ESIZE);
  ret = kv_system_add_sync(key_buf, KEY_LEN, value_buf, 1);
  CHECK(ret == RETURNCODE_ESIZE);
  ret = kv_system_update_sync(key_buf, KEY_LEN, value_buf, 1);
  CHECK(ret == RETURNCODE_ESIZE);

  ret = kv_system_delete_sync(key_buf, KEY_LEN);
  CHECK(ret == RETURNCODE_ESIZE);

  return true;
}

static bool test_set_value_too_long(void) {
  int ret;

  key_buf[0] = 'K';
  ret        = kv_system_set_sync(key_buf, 1, value_buf, DATA_LEN);
  CHECK(ret == RETURNCODE_ESIZE);

  return true;
}

static bool test_get_not_found(void) {
  int ret;
  char random_key[] = "UOR_knaOI#@GUab09578anqOOI$%";
  strcpy((char*) key_buf, random_key);

  uint32_t value_len = 0;
  ret = kv_system_get_sync(key_buf, strlen(random_key), data_buf, DATA_LEN, &value_len);
  CHECK(ret == RETURNCODE_ENOSUPPORT);

  return true;
}

static bool test_add(void) {
  int ret;
  char key[] = "kvtestapp";
  strcpy((char*) key_buf, key);

  ret = kv_system_delete_sync(key_buf, strlen(key));
  CHECK(ret == RETURNCODE_SUCCESS || ret == RETURNCODE_ENOSUPPORT);

  uint32_t value_len = 10;
  for (uint32_t i = 0; i < value_len; i++) {
    value_buf[i] = (uint8_t) i;
  }

  ret = kv_system_add_sync(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  return true;
}

static bool test_add_add(void) {
  int ret;
  char key[] = "kvtestapp";
  strcpy((char*) key_buf, key);

  ret = kv_system_delete_sync(key_buf, strlen(key));
  CHECK(ret == RETURNCODE_SUCCESS || ret == RETURNCODE_ENOSUPPORT);

  uint32_t value_len = 10;
  for (uint32_t i = 0; i < value_len; i++) {
    value_buf[i] = (uint8_t) i;
  }

  ret = kv_system_add_sync(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  ret = kv_system_add_sync(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_ENOSUPPORT);

  return true;
}

static bool test_update(void) {
  int ret;
  char key[] = "kvtestapp";
  strcpy((char*) key_buf, key);

  uint32_t value_len = 10;
  for (uint32_t i = 0; i < value_len; i++) {
    value_buf[i] = (uint8_t) i;
  }

  ret = kv_system_set_sync(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  for (uint32_t i = 0; i < value_len; i++) {
    value_buf[i] = (uint8_t) (10 - i);
  }

  ret = kv_system_update_sync(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  ret = kv_system_get_sync(key_buf, strlen(key), data_buf, 100, &value_len);
  CHECK(ret == RETURNCODE_SUCCESS);
  CHECK(value_len == 10);
  for (uint32_t i = 0; i < 10; i++) {
    CHECK(value_buf[i] == (uint8_t) (10 - i));
  }

  return true;
}

static bool test_update_no_exist(void) {
  int ret;
  char key[] = "kvtestapp";
  strcpy((char*) key_buf, key);

  ret = kv_system_delete_sync(key_buf, strlen(key));
  CHECK(ret == RETURNCODE_SUCCESS || ret == RETURNCODE_ENOSUPPORT);

  uint32_t value_len = 10;
  for (uint32_t i = 0; i < value_len; i++) {
    value_buf[i] = (uint8_t) i;
  }

  ret = kv_system_update_sync(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_ENOSUPPORT);

  return true;
}

static bool test_delete(void) {
  int ret;
  char key[] = "kvtestapp";
  strcpy((char*) key_buf, key);

  uint32_t value_len = 10;
  for (uint32_t i = 0; i < value_len; i++) {
    value_buf[i] = (uint8_t) i;
  }

  ret = kv_system_set_sync(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  ret = kv_system_delete_sync(key_buf, strlen(key));
  CHECK(ret == RETURNCODE_SUCCESS);

  ret = kv_system_get_sync(key_buf, strlen(key), data_buf, 100, &value_len);
  CHECK(ret == RETURNCODE_ENOSUPPORT);

  return true;
}

static bool test_delete_delete(void) {
  int ret;
  char key[] = "kvtestapp";
  strcpy((char*) key_buf, key);

  uint32_t value_len = 10;
  for (uint32_t i = 0; i < value_len; i++) {
    value_buf[i] = (uint8_t) i;
  }

  ret = kv_system_set_sync(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  ret = kv_system_delete_sync(key_buf, strlen(key));
  CHECK(ret == RETURNCODE_SUCCESS);

  ret = kv_system_delete_sync(key_buf, strlen(key));
  CHECK(ret == RETURNCODE_ENOSUPPORT);

  return true;
}

static bool subtest_set_get_region(uint32_t start, uint32_t stop) {
  const char * keys[] = {
    "kvtestappak",
    "kvtestappab",
    "kvtestappaB",
    "kvtestappbw",
    "kvtestappao",
    "kvtestappai",
    "kvtestappat",
    "kvtestappbe",
    "kvtestappby",
    "kvtestappaP",
    "kvtestappaf",
    "kvtestappag",
    "kvtestappau",
    "kvtestappaq",
    "kvtestappbu",
    "kvtestappaJ",
    "kvtestappad",
    "kvtestappal",
    "kvtestappac",
    "kvtestappbS",
    "kvtestappaA",
    "kvtestappbs",
    "kvtestappan",
    "kvtestappa5",
    "kvtestappaI",
    "kvtestappap",
    "kvtestappas",
    "kvtestappaa",
    "kvtestappaj",
    "kvtestappah",
    "kvtestappav",
    "kvtestappa7",
  };

  int ret;

  for (uint32_t i = start; i < stop; i++) {
    // printf ("%d: %s\n", i, array[i]);

    strcpy((char*) key_buf, keys[i]);

    uint32_t value_len = i + 32;
    for (uint32_t j = 0; j < value_len; j++) {
      value_buf[j] = (uint8_t) (j + i);
    }

    ret = kv_system_set_sync(key_buf, strlen(keys[i]), value_buf, value_len);
    CHECK(ret == RETURNCODE_SUCCESS);

    ret = kv_system_get_sync(key_buf, strlen(keys[i]), data_buf, DATA_LEN, &value_len);
    CHECK(ret == RETURNCODE_SUCCESS);
    CHECK(value_len == i + 32);
    for (uint32_t j = 0; j < value_len; j++) {
      CHECK(value_buf[j] == (uint8_t) (j + i));
    }

  }
  return true;
}

// Special test for TicKV with siphash 24 to store keys to all regions,
// assuming 32 regions.

static bool test_set_get_32regions_1(void) {
  return subtest_set_get_region(0, 4);
}
static bool test_set_get_32regions_2(void) {
  return subtest_set_get_region(4, 8);
}
static bool test_set_get_32regions_3(void) {
  return subtest_set_get_region(8, 12);
}
static bool test_set_get_32regions_4(void) {
  return subtest_set_get_region(12, 16);
}
static bool test_set_get_32regions_5(void) {
  return subtest_set_get_region(16, 20);
}
static bool test_set_get_32regions_6(void) {
  return subtest_set_get_region(20, 24);
}
static bool test_set_get_32regions_7(void) {
  return subtest_set_get_region(24, 28);
}
static bool test_set_get_32regions_8(void) {
  return subtest_set_get_region(28, 32);
}

int main(void) {
  unit_test_fun tests[] = {
    TEST(exists),
    TEST(set_get),
    TEST(set_get_too_long),
    TEST(key_too_long),
    TEST(set_value_too_long),
    TEST(get_not_found),
    TEST(add),
    TEST(add_add),
    TEST(update),
    TEST(update_no_exist),
    TEST(delete),
    TEST(delete_delete),
    TEST(set_get_32regions_1),
    TEST(set_get_32regions_2),
    TEST(set_get_32regions_3),
    TEST(set_get_32regions_4),
    TEST(set_get_32regions_5),
    TEST(set_get_32regions_6),
    TEST(set_get_32regions_7),
    TEST(set_get_32regions_8),
  };
  unit_test_runner(tests, sizeof(tests) / sizeof(unit_test_fun), 2000, "org.tockos.unit_test");
  return 0;
}
