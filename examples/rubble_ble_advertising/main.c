#include <gap.h>
#include <rubble_ble.h>
#include <stdbool.h>
#include <stdio.h>
#include <tock.h>

// Sizes in bytes
#define DEVICE_NAME_SIZE 10

/*******************************************************************************
 * MAIN
 ******************************************************************************/

int main(void) {
  int err;
  printf("[Tutorial] BLE Advertising\n");

  // declarations of variables to be used in this BLE example application
  uint16_t advertising_interval_ms = 300;
  uint8_t device_name[]            = "Rubble App";

  static uint8_t adv_data_buf[ADV_DATA_MAX_SIZE];

  // configure advertisement interval to 300ms
  // configure LE only and discoverable
  printf(" - Initializing BLE... %s\n", device_name);
  AdvData_t adv_data = gap_adv_data_new(adv_data_buf, sizeof(adv_data_buf));

  gap_add_flags(&adv_data, LE_GENERAL_DISCOVERABLE | BREDR_NOT_SUPPORTED);

  // configure device name as TockOS
  printf(" - Setting the device name... %s\n", device_name);
  err = gap_add_device_name(&adv_data, device_name, DEVICE_NAME_SIZE);
  if (err < TOCK_SUCCESS)
    printf("rubble_advertise_name, error: %s\r\n", tock_strerror(err));

  // start advertising
  printf(" - Begin advertising! %s\n", device_name);
  err = rubble_start_advertising(adv_data.buf, adv_data.offset, advertising_interval_ms);
  if (err < TOCK_SUCCESS)
    printf("rubble_start_advertising, error: %s\r\n", tock_strerror(err));

  // configuration complete
  printf("Now advertising every %d ms as '%s'\n", advertising_interval_ms,
         device_name);
  return 0;
}
