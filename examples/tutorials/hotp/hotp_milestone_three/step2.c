#include <stdio.h>
#include <string.h>

#include <libtock-sync/storage/kv.h>
#include <libtock/tock.h>

#include "step0.h"
#include "step1.h"
#include "step2.h"



int initialize_key(hotp_key_t* hotp_key, int index) {
  int ret;

  uint8_t key[64];
  int key_len = 0;
  uint8_t value[sizeof(hotp_key_t)];
  uint32_t value_len = 0;

  key_len = snprintf((char*) key, 64, "hotp-key-%i", index);

  ret = libtocksync_kv_get(key, key_len, value, sizeof(hotp_key_t), &value_len);

  // Check if we read what looks like a valid key.
  if (ret == RETURNCODE_SUCCESS && value_len == sizeof(hotp_key_t)) {
    // Looks valid
    memcpy(hotp_key, value, sizeof(hotp_key_t));
  }

  return RETURNCODE_SUCCESS;
}

void save_key(hotp_key_t* hotp_key, int index) {
  int ret;

  // Key is "hotp-key-[index]". Value is the `hotp_key_t` data.
  uint8_t key[64];
  int key_len = 0;
  uint8_t value[sizeof(hotp_key_t)];

  key_len = snprintf((char*) key, 64, "hotp-key-%i", index);

  // Save the key value.
  memcpy(value, hotp_key, sizeof(hotp_key_t));
  ret = libtocksync_kv_set(key, key_len, value, sizeof(hotp_key_t));

  if (ret != 0) {
    printf("ERROR(%i): %s.\r\n", ret, tock_strrcode(ret));
    printf("  Could not store key.\r\n");
  }
}