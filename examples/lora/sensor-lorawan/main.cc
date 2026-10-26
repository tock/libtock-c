/*
   RadioLib Non-Arduino Tock Library LoRaWAN test application

   Licensed under the MIT or Apache License

   Copyright (c) 2023 Alistair Francis <alistair@alistair23.me>
 */

// include the library
#include <RadioLib.h>

// include the hardware abstraction layer
#include "libtockHal.h"

// Include some libtock-c helpers
#include <libtock-sync/sensors/humidity.h>
#include <libtock-sync/sensors/temperature.h>
#include <libtock-sync/storage/kv.h>

#include "CayenneLPP.h"

/* These need to be updated to use values from your LoRaWAN server */
uint64_t joinEUI   = 0x0000000000000000;
uint64_t devEUI    = 0x0000000000000000;
uint8_t appKey[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t nwkKey[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// regional choices: EU868, US915, AU915, AS923, IN865, KR920, CN780, CN500
const LoRaWANBand_t* Region = &AU915;
const uint8_t subBand       = 2;

#define MAX_PAYLOAD_SIZE 10

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

// Retrieve the joinEUI from the Tock K/V store
static int retrieve_join_eui(void) {
  uint32_t value_len;
  returncode_t ret;

  if (!libtock_kv_exists()) {
    return 1;
  }

  ret = libtocksync_kv_get(join_eui_key_buf, JOIN_EUI_KEY_LEN, kv_data_buf, KV_DATA_LEN, &value_len);

  if (ret == RETURNCODE_SUCCESS) {
    // We found the key, set the global variable
    joinEUI = (uint64_t) kv_data_buf[0] |
              ((uint64_t) kv_data_buf[1] << 8) |
              ((uint64_t) kv_data_buf[2] << 16) |
              ((uint64_t) kv_data_buf[3] << 24) |
              ((uint64_t) kv_data_buf[4] << 32) |
              ((uint64_t) kv_data_buf[5] << 40) |
              ((uint64_t) kv_data_buf[6] << 48) |
              ((uint64_t) kv_data_buf[7] << 56);
    return 0;
  } else {
    return 1;
  }
}

// Retrieve the devEUI from the Tock K/V store
static int retrieve_dev_eui(void) {
  uint32_t value_len;
  returncode_t ret;

  if (!libtock_kv_exists()) {
    return 1;
  }

  ret = libtocksync_kv_get(dev_eui_key_buf, DEV_EUI_KEY_LEN, kv_data_buf, KV_DATA_LEN, &value_len);

  if (ret == RETURNCODE_SUCCESS) {
    // We found the key, set the global variable
    devEUI = (uint64_t) kv_data_buf[0] |
             ((uint64_t) kv_data_buf[1] << 8) |
             ((uint64_t) kv_data_buf[2] << 16) |
             ((uint64_t) kv_data_buf[3] << 24) |
             ((uint64_t) kv_data_buf[4] << 32) |
             ((uint64_t) kv_data_buf[5] << 40) |
             ((uint64_t) kv_data_buf[6] << 48) |
             ((uint64_t) kv_data_buf[7] << 56);
    return 0;
  } else {
    return 1;
  }
}

// Retrieve the nwkKey from the Tock K/V store
static int retrieve_nwk_key(void) {
  uint32_t value_len;
  returncode_t ret;

  if (!libtock_kv_exists()) {
    return 1;
  }

  ret = libtocksync_kv_get(nwk_key_key_buf, NWK_KEY_KEY_LEN, nwkKey, 16, &value_len);

  if (ret == RETURNCODE_SUCCESS) {
    return 0;
  } else {
    return 1;
  }
}

// Retrieve the appKey from the Tock K/V store
static int retrieve_app_key(void) {
  uint32_t value_len;
  returncode_t ret;

  if (!libtock_kv_exists()) {
    return 1;
  }

  ret = libtocksync_kv_get(app_key_key_buf, APP_KEY_KEY_LEN, appKey, 16, &value_len);

  if (ret == RETURNCODE_SUCCESS) {
    return 0;
  } else {
    return 1;
  }
}

// Retrieve the LoRaWAN keys from the Tock K/V store
static int retrieve_keys(void) {
  if (retrieve_join_eui() == 0) {
    printf("Retrieve joinEUI key from storage: 0x%lx%lx\r\n",
           (uint32_t)(joinEUI >> 32), (uint32_t)joinEUI);
  } else {
    printf("Unable to retrieve joinEUI key from storage\r\n");
    return 1;
  }

  if (retrieve_dev_eui() == 0) {
    printf("Retrieve devEUI key from storage: 0x%lx%lx\r\n",
           (uint32_t)(devEUI >> 32), (uint32_t)devEUI);
  } else {
    printf("Unable to retrieve devEUI key from storage\r\n");
    return 1;
  }

  if (retrieve_nwk_key() == 0) {
    printf("Retrieve nwkKey key from storage\r\n");
  } else {
    printf("Unable to retrieve nwkKey key from storage\r\n");
    return 1;
  }

  if (retrieve_app_key() == 0) {
    printf("Retrieve appKey key from storage\r\n");
  } else {
    printf("Unable to retrieve appKey key from storage\r\n");
    return 1;
  }

  return 0;
}

// the entry point for the program
int main(void) {
  CayenneLPP Payload(MAX_PAYLOAD_SIZE);

  // Retrieve the LoRaWAN keys from the Tock K/V store
  if (retrieve_keys()) {
    return 1;
  }

  printf("[SX1261] Initialising Radio ... \r\n");

  // create a new instance of the HAL class
  TockRadioLibHal* hal = new TockRadioLibHal();
  int state;

  // now we can create the radio module
  // pinout corresponds to the SparkFun LoRa Thing Plus - expLoRaBLE
  // NSS pin:   0
  // DIO1 pin:  2
  // NRST pin:  4
  // BUSY pin:  1
  SX1262 tock_module = new Module(hal, RADIOLIB_RADIO_NSS, RADIOLIB_RADIO_DIO_1, RADIOLIB_RADIO_RESET,
                                  RADIOLIB_RADIO_BUSY);
  LoRaWANNode node(&tock_module, Region, subBand);

  // Setup the radio
  // The settings here work for the SparkFun LoRa Thing Plus - expLoRaBLE
  node.scanGuard   = 30;
  tock_module.XTAL = true;
  state = tock_module.begin(915.0);

  if (state != RADIOLIB_ERR_NONE) {
    printf("begin failed, code %d\r\n", state);
    return 1;
  }

  node.beginOTAA(joinEUI, devEUI, nwkKey, appKey);

  state = node.activateOTAA();

  if (state != RADIOLIB_LORAWAN_NEW_SESSION) {
    printf("activateOTAA failed, code %d\r\n", state);
    return 1;
  }

  printf("success!\r\n");

  hal->detachInterrupt(RADIOLIB_RADIO_DIO_1);
  hal->pinMode(RADIOLIB_RADIO_DIO_1, TOCK_RADIOLIB_PIN_INPUT);

  int temp = 0;
  int humi = 0;

  // loop forever
  for ( ;;) {
    Payload.reset();

    // Read some sensor data from the board
    libtocksync_temperature_read(&temp);
    libtocksync_humidity_read(&humi);

    Payload.addTemperature(0, temp);
    Payload.addRelativeHumidity(0, humi);

    printf("[SX1261] Transmitting\r\n");

    state = node.sendReceive(Payload.getBuffer(), Payload.getSize());

    if (state >= 0) {
      // the packet was successfully transmitted
      printf("success!\r\n");
    } else {
      printf("failed, code %d\r\n", state);
    }

    printf("Waiting 60 minutes before transmitting again\r\n");
    for (int i = 0; i < 60; i++) {
      hal->delay(60 * 1000);
    }
  }

  return 0;
}
