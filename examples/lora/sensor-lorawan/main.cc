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

#include "CayenneLPP.h"

// To get this working copy radioConfig_example.h to radioConfig.h
// and then modify it to match the LoRaWAN gateway settings.
#ifdef RADIO_CONFIG_CI
#include "radioConfig_example.h"
#else
#include "radioConfig.h"
#endif

#define MAX_SIZE 10

// the entry point for the program
int main(void) {
  CayenneLPP Payload(MAX_SIZE);

  printf("[SX1261] Initialising Radio ... \r\n");

  // create a new instance of the HAL class
  TockHal* hal = new TockHal();
  int state;

  // now we can create the radio module
  // pinout corresponds to the SparkFun LoRa Thing Plus - expLoRaBLE
  // NSS pin:   0
  // DIO1 pin:  2
  // NRST pin:  4
  // BUSY pin:  1
  SX1262 tock_module = new Module(hal, RADIO_NSS, RADIO_DIO_1, RADIO_RESET, RADIO_BUSY);
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

  hal->detachInterrupt(RADIO_DIO_1);
  hal->pinMode(RADIO_DIO_1, PIN_INPUT);

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

    if (state > 0) {
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
