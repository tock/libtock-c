#include <esp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LEN 1024

int main(void) {
  int ret;

  printf("[Brad Wifi]\n");
  // printf("This app will try to get the status from a radio.\n\n");

  // char* at_status_command = "AT+GMR\r\n";
  // char* at_status_command = "AT\r\n";
  // uint8_t tx_buffer[64] = {0};
  uint8_t recv_buffer[BUFFER_LEN] = {0};

  // ret = get_chip_version(recv_buffer, BUFFER_LEN);
  // if (ret < 0) {
  //  printf("get_chip_version failed %i\n", ret);
  //  return -1;
  // }
  // printf("Version Info:\n%s\n\n", recv_buffer);

  // ret = esp32_get_station_mac_address(recv_buffer, BUFFER_LEN);
  // if (ret < 0) {
  //   printf("get_mac_address failed %i\n", ret);
  //   return -1;
  // }
  // printf("MAC address: %s\n", recv_buffer);

  ret = esp32_get_connection_status();
  if (ret < 0) {
    printf("get_connection_status failed %i\n", ret);
    return -1;
  }

  if (ret != ESP32_CONNECTION_STATUS_CONNECTED) {

    ret = esp32_set_mode(ESP32_MODE_STATION);
    if (ret < 0) {
      printf("set_mode failed %i\n", ret);
      return -1;
    }
    printf("set_mode to station.\n");

    ret = esp32_get_mode();
    if (ret < 0) {
      printf("get_mode failed %i\n", ret);
      return -1;
    }
    ret = esp32_mode2str(ret, recv_buffer, BUFFER_LEN);
    if (ret <= 0) {
      printf("mode2str failed %i\n", ret);
      return -1;
    }
    printf("Mode: %s\n", recv_buffer);

    // Do a few tries to connect to Wi-Fi.
    int wifi_connect_tries = 0;
    for (wifi_connect_tries = 0; wifi_connect_tries < 5; wifi_connect_tries++) {
      ret = esp32_connect_to_network("ssid", "pword");
      if (ret < 0) {
        printf("connect to network failed %i\n", ret);
      } else {
        break;
      }
    }
    if (wifi_connect_tries == 5) {
      printf("Could not connect to Wi-Fi\n");
      return -1;
    }
    printf("Successfully connected to Wi-Fi network\n");

    ret = esp32_get_connection_status();
    if (ret < 0) {
      printf("get_connection_status failed %i\n", ret);
      return -1;
    }
    ret = esp32_connection_status2str(ret, recv_buffer, BUFFER_LEN);
    if (ret <= 0) {
      printf("esp32_connection_status2str failed %i\n", ret);
      return -1;
    }
    printf("Connection status: %s\n", recv_buffer);

    ret = esp32_get_ip_address(recv_buffer, BUFFER_LEN);
    if (ret < 0) {
      printf("get ip address failed %i\n", ret);
      return -1;
    }
    printf("ip address: %s\n", recv_buffer);
  }

  ret = esp32_http_get("http://postman-echo.com/get?foo1=bar1&foo2=bar2", recv_buffer, BUFFER_LEN);
  if (ret < 0) {
    printf("HTTP GET failed %i\n", ret);
    return -1;
  }
  printf("response: %s\n", recv_buffer);

  return 0;
}
