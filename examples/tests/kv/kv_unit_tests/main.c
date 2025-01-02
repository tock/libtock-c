#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/services/unit_test.h>
#include <libtock-sync/storage/kv.h>

#define KEY_LEN  200
#define DATA_LEN 3000

uint8_t key_buf[KEY_LEN];
uint8_t data_buf[DATA_LEN];
uint8_t value_buf[DATA_LEN];


static bool test_exists(void) {
  bool ret = libtock_kv_exists();
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

  ret = libtocksync_kv_set(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  ret = libtocksync_kv_get(key_buf, strlen(key), data_buf, DATA_LEN, &value_len);
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

  ret = libtocksync_kv_set(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  ret = libtocksync_kv_get(key_buf, strlen(key), data_buf, 2, &value_len);
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
  ret = libtocksync_kv_get(key_buf, KEY_LEN, value_buf, 50, &value_len);
  CHECK(ret == RETURNCODE_ESIZE);

  value_buf[0] = 'V';
  ret = libtocksync_kv_set(key_buf, KEY_LEN, value_buf, 1);
  CHECK(ret == RETURNCODE_ESIZE);
  ret = libtocksync_kv_add(key_buf, KEY_LEN, value_buf, 1);
  CHECK(ret == RETURNCODE_ESIZE);
  ret = libtocksync_kv_update(key_buf, KEY_LEN, value_buf, 1);
  CHECK(ret == RETURNCODE_ESIZE);

  ret = libtocksync_kv_delete(key_buf, KEY_LEN);
  CHECK(ret == RETURNCODE_ESIZE);

  return true;
}

static bool test_set_value_too_long(void) {
  int ret;

  key_buf[0] = 'K';
  ret        = libtocksync_kv_set(key_buf, 1, value_buf, DATA_LEN);
  CHECK(ret == RETURNCODE_ESIZE);

  return true;
}

static bool test_get_not_found(void) {
  int ret;
  char random_key[] = "UOR_knaOI#@GUab09578anqOOI$%";
  strcpy((char*) key_buf, random_key);

  uint32_t value_len = 0;
  ret = libtocksync_kv_get(key_buf, strlen(random_key), data_buf, DATA_LEN, &value_len);
  CHECK(ret == RETURNCODE_ENOSUPPORT);

  return true;
}

static bool test_get_not_found2(void) {
  int ret;
  char random_key[] = "HKANfdlksiqiovcHIFoihASIUcc";
  strcpy((char*) key_buf, random_key);

  uint32_t value_len = 0;
  ret = libtocksync_kv_get(key_buf, strlen(random_key), data_buf, DATA_LEN, &value_len);
  CHECK(ret == RETURNCODE_ENOSUPPORT);

  return true;
}

static bool test_add(void) {
  int ret;
  char key[] = "kvtestapp";
  strcpy((char*) key_buf, key);

  ret = libtocksync_kv_delete(key_buf, strlen(key));
  CHECK(ret == RETURNCODE_SUCCESS || ret == RETURNCODE_ENOSUPPORT);

  uint32_t value_len = 10;
  for (uint32_t i = 0; i < value_len; i++) {
    value_buf[i] = (uint8_t) i;
  }

  ret = libtocksync_kv_add(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  return true;
}

static bool test_add_add(void) {
  int ret;
  char key[] = "kvtestapp";
  strcpy((char*) key_buf, key);

  ret = libtocksync_kv_delete(key_buf, strlen(key));
  CHECK(ret == RETURNCODE_SUCCESS || ret == RETURNCODE_ENOSUPPORT);

  uint32_t value_len = 10;
  for (uint32_t i = 0; i < value_len; i++) {
    value_buf[i] = (uint8_t) i;
  }

  ret = libtocksync_kv_add(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  ret = libtocksync_kv_add(key_buf, strlen(key), value_buf, value_len);
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

  ret = libtocksync_kv_set(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  for (uint32_t i = 0; i < value_len; i++) {
    value_buf[i] = (uint8_t) (10 - i);
  }

  ret = libtocksync_kv_update(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  ret = libtocksync_kv_get(key_buf, strlen(key), data_buf, 100, &value_len);
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

  ret = libtocksync_kv_delete(key_buf, strlen(key));
  CHECK(ret == RETURNCODE_SUCCESS || ret == RETURNCODE_ENOSUPPORT);

  uint32_t value_len = 10;
  for (uint32_t i = 0; i < value_len; i++) {
    value_buf[i] = (uint8_t) i;
  }

  ret = libtocksync_kv_update(key_buf, strlen(key), value_buf, value_len);
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

  ret = libtocksync_kv_set(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  ret = libtocksync_kv_delete(key_buf, strlen(key));
  CHECK(ret == RETURNCODE_SUCCESS);

  ret = libtocksync_kv_get(key_buf, strlen(key), data_buf, 100, &value_len);
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

  ret = libtocksync_kv_set(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  ret = libtocksync_kv_delete(key_buf, strlen(key));
  CHECK(ret == RETURNCODE_SUCCESS);

  ret = libtocksync_kv_delete(key_buf, strlen(key));
  CHECK(ret == RETURNCODE_ENOSUPPORT);

  return true;
}

static bool test_add_update_set(void) {
  int ret;
  char key[] = "kvtestapp";
  strcpy((char*) key_buf, key);

  ret = libtocksync_kv_delete(key_buf, strlen(key));
  CHECK(ret == RETURNCODE_SUCCESS || ret == RETURNCODE_ENOSUPPORT);

  uint32_t value_len = 30;
  for (uint32_t i = 0; i < value_len; i++) {
    value_buf[i] = (uint8_t) i;
  }

  ret = libtocksync_kv_add(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  value_len = 15;
  for (uint32_t i = 0; i < value_len; i++) {
    value_buf[i] = (uint8_t) i + 10;
  }

  ret = libtocksync_kv_update(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  value_len    = 2;
  value_buf[0] = 'H';
  value_buf[1] = 'I';

  ret = libtocksync_kv_set(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  ret = libtocksync_kv_get(key_buf, strlen(key), data_buf, 100, &value_len);
  CHECK(ret == RETURNCODE_SUCCESS);
  CHECK(value_len == 2);
  CHECK(value_buf[0] == 'H');
  CHECK(value_buf[1] == 'I');

  return true;
}

static bool test_set_zero_value(void) {
  int ret;
  char key[] = "kvtestapp";
  strcpy((char*) key_buf, key);

  uint32_t value_len = 0;
  ret = libtocksync_kv_set(key_buf, strlen(key), value_buf, value_len);
  CHECK(ret == RETURNCODE_SUCCESS);

  return true;
}

static bool subtest_set_get_region(uint32_t start, uint32_t stop) {
  const char* keys[] = {
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
    strcpy((char*) key_buf, keys[i]);

    uint32_t value_len = i + 32;
    for (uint32_t j = 0; j < value_len; j++) {
      value_buf[j] = (uint8_t) (j + i);
    }

    ret = libtocksync_kv_set(key_buf, strlen(keys[i]), value_buf, value_len);
    CHECK(ret == RETURNCODE_SUCCESS);

    ret = libtocksync_kv_get(key_buf, strlen(keys[i]), data_buf, DATA_LEN, &value_len);
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

static bool test_garbage_collect(void) {
  int ret;
  char key[] = "kvtestapp";
  strcpy((char*) key_buf, key);
  int num_of_keys    = 0;
  uint32_t value_len = 500;

  printf("Filling up storage with invalid (deleted) keys\r\n");

  // Fill up the storage
  do {
    for (uint32_t i = 0; i < value_len; i++) {
      value_buf[i] = num_of_keys;
    }

    printf("Setting key %d\r\n", num_of_keys);
    ret = libtocksync_kv_set(key_buf, strlen(key), value_buf, value_len);

    if (ret == RETURNCODE_SUCCESS) {
      num_of_keys++;
    }
  } while (ret == RETURNCODE_SUCCESS);

  printf("Wrote %d K/V entries to flash, running garbage collection\n", num_of_keys);

  ret = libtocksync_kv_garbage_collect();
  CHECK(ret == RETURNCODE_SUCCESS);

  printf("Garbage collection finished, trying to re-set the keys\r\n");

  for (int i = 0; i < num_of_keys; i++) {
    printf("Setting key %d\r\n", i);
    for (uint32_t j = 0; j < value_len; j++) {
      value_buf[j] = i;
    }

    CHECK(libtocksync_kv_set(key_buf, strlen(key), value_buf, value_len) == RETURNCODE_SUCCESS);
  }

  printf("Set all keys\r\n");

  return true;
}

int main(void) {
  unit_test_fun tests[] = {
    TEST(exists),
    TEST(set_get),
    TEST(set_get_too_long),
    TEST(key_too_long),
    TEST(set_value_too_long),
    TEST(get_not_found),
    TEST(get_not_found2),
    TEST(add),
    TEST(add_add),
    TEST(update),
    TEST(update_no_exist),
    TEST(delete),
    TEST(delete_delete),
    TEST(add_update_set),
    TEST(set_zero_value),
    TEST(set_get_32regions_1),
    TEST(set_get_32regions_2),
    TEST(set_get_32regions_3),
    TEST(set_get_32regions_4),
    TEST(set_get_32regions_5),
    TEST(set_get_32regions_6),
    TEST(set_get_32regions_7),
    TEST(set_get_32regions_8),
    TEST(garbage_collect),
  };
  unit_test_runner(tests, sizeof(tests) / sizeof(unit_test_fun), 2000, "org.tockos.unit_test");
  return 0;
}
