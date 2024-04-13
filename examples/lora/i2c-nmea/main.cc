#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "minmea.h"
#include <console.h>
#include <nmea.h>

// include the library
#include <RadioLib.h>

// include the hardware abstraction layer
#include "libtockHal.h"

#define NMEA_BUFFER_LEN 128
#define TRANSMIT_BUFFER_LEN 64

uint8_t nmea_buf[NMEA_BUFFER_LEN];
char trans_buf[TRANSMIT_BUFFER_LEN];
char date_buf[TRANSMIT_BUFFER_LEN];

int main(void) {
  int nmea_offset;
  int ret = 0;

  printf("Staring I2C NMEA\r\n");

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

  bzero(date_buf, TRANSMIT_BUFFER_LEN);

  while (1) {
    nmea_offset = nmea_read_sentance_sync(nmea_buf, NMEA_BUFFER_LEN);

    if (nmea_offset <= 0) {
      printf("Error reading NMEA data\r\n");
      hal->delay(60 * 1000);

      continue;
    }

    // Ensure it's a valid C string
    nmea_buf[nmea_offset] = '\0';

    // We only parse a few data points
    // See https://github.com/kosma/minmea/blob/master/example.c for details
    // on collecting more information
    switch (minmea_sentence_id((char *) nmea_buf, false)) {
      case MINMEA_SENTENCE_RMC: {
        struct minmea_sentence_rmc frame;

        if (minmea_parse_rmc(&frame, (char *) nmea_buf)) {
          if (frame.longitude.value == 0 || frame.latitude.value == 0 || !frame.valid) {
            // Not a valid location or fix
            printf("Waiting for a fix\r\n");
            hal->delay(10 * 1000);
            continue;
          }

          bzero(trans_buf, TRANSMIT_BUFFER_LEN);
          ret = snprintf(trans_buf, TRANSMIT_BUFFER_LEN, "time: %02d:%02d:%02d",
            frame.time.hours, frame.time.minutes, frame.time.seconds);
          if (ret > 0) {
            printf("Transmitting: %s\r\n", trans_buf);
            state = radio->transmit(trans_buf);
          }

          // Ensure this data has been sent
          hal->delay(200);

          ret = snprintf(trans_buf, TRANSMIT_BUFFER_LEN, "date: %d.%02d.%02d",
            frame.date.year, frame.date.month, frame.date.day);
          if (ret > 0) {
            printf("Transmitting: %s\r\n", trans_buf);
            state |= radio->transmit(trans_buf);
          }
          // GGA doesn't supply the date, so copy it for GGA use
          memcpy(date_buf, trans_buf, ret);

          // Ensure this data has been sent
          hal->delay(200);

          ret = snprintf(trans_buf, TRANSMIT_BUFFER_LEN, "lat: (%ld/%ld)",
            frame.latitude.value, frame.latitude.scale);
          if (ret > 0) {
            printf("Transmitting: %s\r\n", trans_buf);
            state |= radio->transmit(trans_buf);
          }

          // Ensure this data has been sent
          hal->delay(200);

          ret = snprintf(trans_buf, TRANSMIT_BUFFER_LEN, "lon: (%ld/%ld)",
            frame.longitude.value, frame.longitude.scale);
          if (ret > 0) {
            printf("Transmitting: %s\r\n", trans_buf);
            state |= radio->transmit(trans_buf);
          }

          // Ensure this data has been sent
          hal->delay(200);

          ret = snprintf(trans_buf, TRANSMIT_BUFFER_LEN, "speed: (%ld/%ld)",
            frame.speed.value, frame.speed.scale);
          if (ret > 0) {
            printf("Transmitting: %s\r\n", trans_buf);
            state |= radio->transmit(trans_buf);
          }

          if (state == RADIOLIB_ERR_NONE) {
            // the packet was successfully transmitted
            printf("success!\r\n");
          } else {
            printf("failed, code %d\r\n", state);
          }

          // transmitting uses **LOTS** of power
          // wait before going again
          printf("Sleeping for a minute to save power\r\n");
          hal->delay(60 * 1000);
          printf("Starting again\r\n");
        } else {
          printf("$xxRMC sentence is not parsed\n");
        }
      } break;

      case MINMEA_SENTENCE_GGA: {
        struct minmea_sentence_gga frame;

        if (strlen(date_buf) == 0) {
          // We want to get a RMC packet first to setup the date
          continue;
        }

        if (minmea_parse_gga(&frame, (char *) nmea_buf)) {
          if (frame.longitude.value == 0 || frame.latitude.value == 0 || !frame.fix_quality) {
            // Not a valid location or fix
            printf("Waiting for a fix\r\n");
            hal->delay(10 * 1000);
            continue;
          }

          bzero(trans_buf, TRANSMIT_BUFFER_LEN);
          ret = snprintf(trans_buf, TRANSMIT_BUFFER_LEN, "time: %02d:%02d:%02d",
            frame.time.hours, frame.time.minutes, frame.time.seconds);
          if (ret > 0) {
            printf("Transmitting: %s\r\n", trans_buf);
            state = radio->transmit(trans_buf);
          }

          // Ensure this data has been sent
          hal->delay(200);

          printf("Transmitting: %s\r\n", date_buf);
          state |= radio->transmit(date_buf);

          // Ensure this data has been sent
          hal->delay(200);

          ret = snprintf(trans_buf, TRANSMIT_BUFFER_LEN, "lat: (%ld/%ld)",
            frame.latitude.value, frame.latitude.scale);
          if (ret > 0) {
            printf("Transmitting: %s\r\n", trans_buf);
            state |= radio->transmit(trans_buf);
          }

          // Ensure this data has been sent
          hal->delay(200);

          ret = snprintf(trans_buf, TRANSMIT_BUFFER_LEN, "lon: (%ld/%ld)",
            frame.longitude.value, frame.longitude.scale);
          if (ret > 0) {
            printf("Transmitting: %s\r\n", trans_buf);
            state |= radio->transmit(trans_buf);
          }

          // Ensure this data has been sent
          hal->delay(200);

          ret = snprintf(trans_buf, TRANSMIT_BUFFER_LEN, "altitude: (%ld/%ld)",
            frame.altitude.value, frame.altitude.scale);
          if (ret > 0) {
            printf("Transmitting: %s\r\n", trans_buf);
            state |= radio->transmit(trans_buf);
          }

          if (state == RADIOLIB_ERR_NONE) {
            // the packet was successfully transmitted
            printf("success!\r\n");
          } else {
            printf("failed, code %d\r\n", state);
          }

          // transmitting uses **LOTS** of power
          // wait before going again
          printf("Sleeping for a minute to save power\r\n");
          hal->delay(60 * 1000);
          printf("Starting again\r\n");
        } else {
          printf("$xxGGA sentence is not parsed\r\n");
        }
      } break;

      case MINMEA_SENTENCE_GST:
        break;

      case MINMEA_SENTENCE_GSV:
        break;

      case MINMEA_SENTENCE_VTG:
        break;

      case MINMEA_SENTENCE_ZDA:
        break;

      case MINMEA_INVALID:
        break;

      default:
        break;
    }
  }

  return 0;
}
