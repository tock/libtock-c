/*
   RadioLib Non-Arduino Tock Library LoRaWAN test application

   Licensed under the MIT or Apache License

   Copyright (c) 2023 Alistair Francis <alistair@alistair23.me>
 */

#include <cinttypes>
#include <stdlib.h>

// include the library
#include <RadioLib.h>

// Include some libtock-c helpers
#include <libtock-sync/storage/kv.h>

// To get this working copy radioConfig_example.h to radioConfig.h
// and then modify it to match the LoRaWAN gateway settings.
#ifdef RADIO_CONFIG_CI
#include "radioConfig_example.h"
#else
#include "radioConfig.h"
#endif

#define JOIN_EUI_KEY_LEN  8
uint8_t join_eui_key_buf[JOIN_EUI_KEY_LEN] = "joinEUI";

#define DEV_EUI_KEY_LEN  7
uint8_t dev_eui_key_buf[DEV_EUI_KEY_LEN] = "devEUI";

#define NWK_KEY_KEY_LEN  7
uint8_t nwk_key_key_buf[NWK_KEY_KEY_LEN] = "nwkKey";

#define APP_KEY_KEY_LEN  7
uint8_t app_key_key_buf[APP_KEY_KEY_LEN] = "appKey";

#define KV_DATA_LEN 8
uint8_t kv_data_buf[KV_DATA_LEN];

// Store the joinEUI to the Tock K/V store
static int set_join_eui(void) {
  returncode_t ret;

  if (!libtock_kv_exists()) {
    return 1;
  }

  kv_data_buf[0] = joinEUI & 0xFF;
  kv_data_buf[1] = (joinEUI >> 8) & 0xFF;
  kv_data_buf[2] = (joinEUI >> 16) & 0xFF;
  kv_data_buf[3] = (joinEUI >> 24) & 0xFF;
  kv_data_buf[4] = (joinEUI >> 32) & 0xFF;
  kv_data_buf[5] = (joinEUI >> 40) & 0xFF;
  kv_data_buf[6] = (joinEUI >> 48) & 0xFF;
  kv_data_buf[7] = (joinEUI >> 56) & 0xFF;

  ret = libtocksync_kv_set(join_eui_key_buf, JOIN_EUI_KEY_LEN, kv_data_buf, KV_DATA_LEN);

  if (ret == RETURNCODE_SUCCESS) {
    return 0;
  } else {
    return 1;
  }
}

// Store the devEUI to the Tock K/V store
static int set_dev_eui(void) {
  returncode_t ret;

  if (!libtock_kv_exists()) {
    return 1;
  }

  kv_data_buf[0] = devEUI & 0xFF;
  kv_data_buf[1] = (devEUI >> 8) & 0xFF;
  kv_data_buf[2] = (devEUI >> 16) & 0xFF;
  kv_data_buf[3] = (devEUI >> 24) & 0xFF;
  kv_data_buf[4] = (devEUI >> 32) & 0xFF;
  kv_data_buf[5] = (devEUI >> 40) & 0xFF;
  kv_data_buf[6] = (devEUI >> 48) & 0xFF;
  kv_data_buf[7] = (devEUI >> 56) & 0xFF;

  ret = libtocksync_kv_set(dev_eui_key_buf, DEV_EUI_KEY_LEN, kv_data_buf, KV_DATA_LEN);

  if (ret == RETURNCODE_SUCCESS) {
    return 0;
  } else {
    return 1;
  }
}

// Store the nwkKey to the Tock K/V store
static int set_nwk_key(void) {
  returncode_t ret;

  if (!libtock_kv_exists()) {
    return 1;
  }

  ret = libtocksync_kv_set(nwk_key_key_buf, NWK_KEY_KEY_LEN, nwkKey, 16);

  if (ret == RETURNCODE_SUCCESS) {
    return 0;
  } else {
    return 1;
  }
}

// Store the appKey to the Tock K/V store
static int set_app_key(void) {
  returncode_t ret;

  if (!libtock_kv_exists()) {
    return 1;
  }

  ret = libtocksync_kv_set(app_key_key_buf, APP_KEY_KEY_LEN, appKey, 16);

  if (ret == RETURNCODE_SUCCESS) {
    return 0;
  } else {
    return 1;
  }
}

// the entry point for the program
int main(void) {
  if (set_join_eui() == 0) {
    printf("Set joinEUI key to storage: 0x%lx%lx\r\n",
           (uint32_t)(joinEUI >> 32), (uint32_t)joinEUI);
  } else {
    printf("Unable to store joinEUI key to storage\r\n");
    return 1;
  }

  if (set_dev_eui() == 0) {
    printf("Set devEUI key to storage: 0x%lx%lx\r\n",
           (uint32_t)(devEUI >> 32), (uint32_t)devEUI);
  } else {
    printf("Unable to store devEUI key to storage\r\n");
    return 1;
  }

  if (set_nwk_key() == 0) {
    printf("Set nwkKey key to storage\r\n");
  } else {
    printf("Unable to store nwkKey to storage\r\n");
    return 1;
  }

  if (set_app_key() == 0) {
    printf("Set appKey key to storage\r\n");
  } else {
    printf("Unable to store appKey to storage\r\n");
    return 1;
  }

  return 0;
}
