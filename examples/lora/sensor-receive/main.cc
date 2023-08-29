/*
   RadioLib Non-Arduino Tock Library test application

   Licensed under the MIT or Apache License

   Copyright (c) 2023 Alistair Francis <alistair@alistair23.me>
 */

// include the library
#include <RadioLib.h>

// include the hardware abstraction layer
#include "libtockHal.h"

// Include some libtock-c helpers
#include <humidity.h>
#include <temperature.h>

#define BUFFER_LEN 64

// the entry point for the program
int main(void) {
  char buffer[BUFFER_LEN];

  printf("[SX1261] Initialising Radio ... \n");

  // create a new instance of the HAL class
  TockHal* hal = new TockHal();

  // now we can create the radio module
  // pinout corresponds to the SparkFun LoRa Thing Plus - expLoRaBLE
  // NSS pin:   0
  // DIO1 pin:  2
  // NRST pin:  4
  // BUSY pin:  1
  Module* tock_module = new Module(hal, RADIO_NSS, RADIO_DIO_1, RADIO_RESET, RADIO_BUSY);
  SX1262* radio       = new SX1262(tock_module);

  // Setup the radio
  // The settings here work for the SparkFun LoRa Thing Plus - expLoRaBLE
  radio->XTAL = true;
  int state = radio->begin(915.0);

  if (state != RADIOLIB_ERR_NONE) {
    printf("failed, code %d\r\n", state);
    return 1;
  }
  printf("success!\r\n");

  printf("[SX1261] Receiving...\r\n");

  // loop forever
  for ( ;;) {
    // Ensure there are no pending callbacks
    yield_no_wait();

    state = radio->receive((uint8_t*)buffer, BUFFER_LEN);

    if (state == RADIOLIB_ERR_NONE) {
      // the packet was successfully transmitted
      printf("success!: %s\r\n", buffer);

      // wait for a second before transmitting again
      hal->delay(1000);
    } else {
      printf("failed, code %d\r\n", state);
    }
  }

  return 0;
}
