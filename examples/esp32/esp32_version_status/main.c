#include <at_commands.h>
#include <esp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LEN 512

int main(void) {
  int ret;
  uint8_t recv_buffer[BUFFER_LEN] = {0};

  printf("[ESP32 Status Test]\n");
  printf("This app will try to get the version info from the ESP32.\n\n");

  // Check that the AT commands driver is installed in the kernel.
  bool at_commands_driver_exists = driver_exists(DRIVER_NUM_ATCOMMANDS);
  if (at_commands_driver_exists == false) {
    printf("Error: no AT commands kernel driver found.\n");
    return -1;
  }

  ret = esp32_get_chip_version(recv_buffer, BUFFER_LEN);
  if (ret < 0) {
    printf("get_chip_version failed. Error: %i\n", ret);
    return -1;
  }
  printf("Version Info:\n%s\n\n", recv_buffer);

  printf("Getting MAC address...\n");
  ret = esp32_get_station_mac_address(recv_buffer, BUFFER_LEN);
  if (ret < 0) {
    printf("esp32_get_station_mac_address failed %i\n", ret);
    return -1;
  }
  printf("MAC address: %s\n", recv_buffer);

  return 0;
}
