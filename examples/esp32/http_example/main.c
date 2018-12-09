#include <esp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LEN 1024

// Fill these in with the correct settings for your Wi-Fi network.
const char* SSID     = "wifi";
const char* PASSWORD = "wifipassword";

int main(void) {
  int ret;
  uint8_t recv_buffer[BUFFER_LEN] = {0};

  printf("[HTTP Example]\n");
  printf("Example app to make HTTP requests.\n");

  // Check the current connection status. If we are already connected, great,
  // we can skip a lot of setup.
  ret = esp32_get_connection_status();
  if (ret < 0) {
    printf("get_connection_status failed %i\n", ret); return -1;
  }

  // If not already connected, connect to the Wi-Fi network.
  if (ret != ESP32_CONNECTION_STATUS_CONNECTED) {

    // Make sure that the ESP32 is configured to be a client (station).
    printf("Ensuring the ESP32 is in station mode...");
    ret = esp32_set_mode(ESP32_MODE_STATION);
    if (ret < 0) {
      printf("set_mode failed %i\n", ret);
      return -1;
    }
    printf("done\n");

    // Do a few tries to connect to Wi-Fi.
    printf("Connecting to the Wi-Fi network.\n");
    int wifi_connect_tries = 0;
    for (wifi_connect_tries = 0; wifi_connect_tries < 5; wifi_connect_tries++) {
      ret = esp32_connect_to_network(SSID, PASSWORD);
      if (ret < 0) {
        printf("connect to network failed %i\n", ret);
        if (wifi_connect_tries < 4) {
          printf("retrying...\n");
        }
      } else {
        break;
      }
    }
    if (wifi_connect_tries == 5) {
      printf("Could not connect to Wi-Fi\n");
      return -1;
    }
    printf("Successfully connected to Wi-Fi network\n");

    // Determine the IP address we got.
    printf("Querying our IP address...");
    ret = esp32_get_ip_address(recv_buffer, BUFFER_LEN);
    if (ret < 0) {
      printf("get ip address failed %i\n", ret);
      return -1;
    }
    printf("done\nIP address: %s\n", recv_buffer);
  }

  // After ensuring we are connected, issue the HTTP GET request.
  // This is a test service that will just echo our arguments back to us.
  printf("Making HTTP GET request\n");
  ret = esp32_http_get("http://postman-echo.com/get?foo1=bar1&foo2=bar2", recv_buffer, BUFFER_LEN);
  if (ret < 0) {
    printf("HTTP GET failed %i\n", ret);
    return -1;
  }
  printf("response:\n%s\n\n", recv_buffer);

  // Now do a simple test HTTP POST request.
  printf("Making HTTP POST request\n");
  const char* post_data = "abcde";
  ret = esp32_http_post_form_data("http://postman-echo.com/post",
                                  (uint8_t*) post_data, strlen(post_data), recv_buffer, BUFFER_LEN);
  if (ret < 0) {
    printf("HTTP POST failed %i\n", ret);
    return -1;
  }
  printf("response:\n%s\n\n", recv_buffer);

  return 0;
}
