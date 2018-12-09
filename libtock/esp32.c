#include <stdlib.h>

#include "at_commands.h"
#include "esp32.h"
#include "timer.h"
#include "tock.h"

//
// HELPER FUNCTIONS
//

// Check if the string "OK" is in the buffer.
bool _esp32_is_ok(uint8_t* buffer, uint32_t buffer_length);

// Check if the string "FAIL" is in the buffer.
bool _esp32_is_fail(uint8_t* buffer, uint32_t buffer_length);

// Check if the string "ERROR" is in the buffer.
bool _esp32_is_error(uint8_t* buffer, uint32_t buffer_length);

// Check if the string "IPD" is in the buffer.
bool _esp32_is_ipd(uint8_t* buffer, uint32_t buffer_length);

// Find the index of the character `search` in the buffer, or return -1 if it
// is not found.
int _esp32_index_of(char search, uint8_t* buffer, uint32_t buffer_len);

// Separate URL into its components. This assumes a simple URL.
int _esp32_parse_url(const char* url,
                     bool* https,
                     uint8_t** hostname, uint8_t** path);

// Check if the string "OK" in in the buffer.
bool _esp32_is_ok(uint8_t* buffer, uint32_t buffer_length) {
  for (uint32_t i = 0; i < buffer_length - 1; i++) {
    if (buffer[i] == 0) {
      return false;
    }
    if (buffer[i] == 'O' && buffer[i + 1] == 'K') {
      return true;
    }
  }
  return false;
}

// Check if the string "FAIL" in in the buffer.
bool _esp32_is_fail(uint8_t* buffer, uint32_t buffer_length) {
  for (uint32_t i = 0; i < buffer_length - 3; i++) {
    if (buffer[i] == 0) {
      return false;
    }
    if (buffer[i] == 'F' && buffer[i + 1] == 'A' && buffer[i + 2] == 'I' && buffer[i + 3] == 'L') {
      return true;
    }
  }
  return false;
}

// Check if the string "ERROR" in in the buffer.
bool _esp32_is_error(uint8_t* buffer, uint32_t buffer_length) {
  for (uint32_t i = 0; i < buffer_length - 4; i++) {
    if (buffer[i] == 0) {
      return false;
    }
    if (buffer[i] == 'E' && buffer[i + 1] == 'R' && buffer[i + 2] == 'R' && buffer[i + 3] == 'O' &&
        buffer[i + 4] == 'R') {
      return true;
    }
  }
  return false;
}

// Check if the string "ERROR" in in the buffer.
bool _esp32_is_ipd(uint8_t* buffer, uint32_t buffer_length) {
  for (uint32_t i = 0; i < buffer_length - 2; i++) {
    if (buffer[i] == 0) {
      return false;
    }
    if (buffer[i] == 'I' && buffer[i + 1] == 'P' && buffer[i + 2] == 'D') {
      return true;
    }
  }
  return false;
}

int _esp32_index_of(char search, uint8_t* buffer, uint32_t buffer_len) {
  for (uint32_t i = 0; i < buffer_len; i++) {
    if (buffer[i] == search) {
      return i;
    }
  }
  return -1;
}

//
// PUBLIC FUNCTIONS
//

int esp32_get_chip_version(uint8_t* rx_buffer, uint32_t rx_buffer_length) {
  char command[] = "AT+GMR\r\n";
  int ret;

  ret = at_commands_txrx_sync((uint8_t*) command,
                              strlen(command),
                              strlen(command),
                              rx_buffer,
                              rx_buffer_length);
  if (ret < 0) return ret;

  // Copy bytes so that we can ignore the "AT+GMR\r\r\n" prefix;
  for (uint32_t i = 0; i < (rx_buffer_length - 13); i++) {
    // Check for end condition: "\r\nOK"
    if (rx_buffer[i + 9] == '\r' &&
        rx_buffer[i + 10] == '\n' &&
        rx_buffer[i + 11] == 'O' &&
        rx_buffer[i + 12] == 'K') {
      rx_buffer[i] = 0;
      break;
    }
    rx_buffer[i] = rx_buffer[9 + i];
  }
  return ret;
}

int esp32_get_station_mac_address(uint8_t* rx_buffer, uint32_t rx_buffer_length) {
  char command[] = "AT+CIPSTAMAC?\r\n";
  int ret;

  ret = at_commands_txrx_sync((uint8_t*) command,
                              strlen(command),
                              strlen(command),
                              rx_buffer,
                              rx_buffer_length);
  if (ret < 0) return ret;

  for (int i = 0; i < 17; i++) {
    rx_buffer[i] = rx_buffer[28 + i];
  }
  rx_buffer[17] = 0;

  return TOCK_SUCCESS;
}

int esp32_factory_reset(void) {
  char command[] = "AT+RESTORE\r\n";
  uint8_t rx_buffer[128];
  int ret;
  uint32_t index = 0;

  ret = at_commands_txrx_sync((uint8_t*) command,
                              strlen(command),
                              strlen(command),
                              rx_buffer,
                              128);
  if (ret < 0) return ret;
  index += ret;

  // Do a second receive after the chip has had a chance to reset.
  ret = at_commands_rx_sync(rx_buffer + index, 128 - index);
  if (ret < 0) return ret;
  index += ret;

  for (uint32_t i = 0; i < index; i++) {
    if (rx_buffer[i] == 'O' && rx_buffer[i + 1] == 'K') {
      return TOCK_SUCCESS;
    }
  }

  return TOCK_FAIL;
}

int esp32_get_aps(uint8_t* rx_buffer, uint32_t rx_buffer_length) {
  char command[] = "AT+CWLAP\r\n";
  int ret;
  uint32_t index = 0;

  ret = at_commands_txrx_sync((uint8_t*) command,
                              strlen(command),
                              strlen(command),
                              rx_buffer,
                              rx_buffer_length);
  if (ret < 0) return ret;
  index += ret;
  if (index >= rx_buffer_length) return ret;

  // The first thing we get is useless, so we do another listen to get the
  // actual data.
  ret = at_commands_rx_sync(rx_buffer + index,
                            rx_buffer_length - index);
  if (ret < 0) return ret;
  index += ret;
  if (index >= rx_buffer_length) return ret;

  // We should loop and check for "OK", but leaving that as a TODO.

  return index;
}

int esp32_get_mode(void) {
  char command[] = "AT+CWMODE?\r\n";
  uint8_t rx_buffer[128];
  int ret;

  ret = at_commands_txrx_sync((uint8_t*) command,
                              strlen(command),
                              strlen(command),
                              rx_buffer,
                              128);
  if (ret < 0) return ret;

  // Extract the mode id type
  int id = 0;
  for (int i = 0; i < ret; i++) {
    if (rx_buffer[i] == ':') {
      rx_buffer[i + 2] = 0;
      id = atoi((char*) (rx_buffer + i + 1));
    }
  }

  return id;
}

int esp32_set_mode(esp32_mode_e mode) {
  char tx_buffer[128];
  uint8_t rx_buffer[128];

  int length = snprintf(tx_buffer, 128, "AT+CWMODE=%i\r\n", mode);
  int ret;

  ret = at_commands_txrx_sync((uint8_t*) tx_buffer,
                              128,
                              length,
                              rx_buffer,
                              128);
  if (ret < 0) return ret;

  for (int i = 0; i < ret; i++) {
    if (rx_buffer[i] == 'O' && rx_buffer[i + 1] == 'K') {
      return TOCK_SUCCESS;
    }
  }

  return TOCK_FAIL;
}

int esp32_mode2str(esp32_mode_e mode, uint8_t* buffer, uint32_t buffer_length) {
  int ret = 0;
  switch (mode) {
    case ESP32_MODE_UNKNOWN: ret         = snprintf((char*) buffer, buffer_length, "%s", "error"); break;
    case ESP32_MODE_STATION: ret         = snprintf((char*) buffer, buffer_length, "%s", "station"); break;
    case ESP32_MODE_SOFT_AP: ret         = snprintf((char*) buffer, buffer_length, "%s", "soft AP"); break;
    case ESP32_MODE_STATION_SOFT_AP: ret = snprintf((char*) buffer, buffer_length, "%s", "station+soft AP"); break;
  }
  return ret;
}

int esp32_connect_to_network(const char* ssid, const char* password) {
  char tx_buffer[128];
  uint8_t rx_buffer[256];

  int length = snprintf(tx_buffer, 128, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password);

  int ret;
  int index = 0;

  ret = at_commands_txrx_sync((uint8_t*) tx_buffer,
                              128,
                              length,
                              rx_buffer,
                              256);
  if (ret < 0) return ret;
  index += ret;

  // Check if we got OK or FAIL after asking for the connection.
  if (_esp32_is_ok(rx_buffer, 256) == true) return TOCK_SUCCESS;
  if (_esp32_is_fail(rx_buffer, 256) == true) return TOCK_FAIL;

  // Otherwise we need to loop and receive until we get OK or FAIL.
  while (true) {
    // Do another receive.
    ret = at_commands_rx_sync(rx_buffer + index, 256);
    if (ret < 0) return ret;
    index += ret;
    if (index >= 256) return TOCK_FAIL;

    // Now check again for success or fail
    if (_esp32_is_ok(rx_buffer, 256) == true) return TOCK_SUCCESS;
    if (_esp32_is_fail(rx_buffer, 256) == true) return TOCK_FAIL;
  }
}

int esp32_get_connection_status(void) {
  char command[] = "AT+CIPSTATUS\r\n";
  uint8_t rx_buffer[128];
  int ret;

  ret = at_commands_txrx_sync((uint8_t*) command,
                              strlen(command),
                              strlen(command),
                              rx_buffer,
                              128);
  if (ret < 0) return ret;

  // Extract status number
  int status = 0;
  for (int i = 0; i < ret; i++) {
    if (rx_buffer[i] == ':') {
      rx_buffer[i + 2] = 0;
      status = atoi((char*) (rx_buffer + i + 1));
    }
  }

  return status;
}

int esp32_connection_status2str(esp32_connection_status_e mode, uint8_t* buffer, uint32_t buffer_length) {
  int ret = 0;
  switch (mode) {
    case ESP32_MODE_UNKNOWN:
      ret = snprintf((char*) buffer, buffer_length, "%s", "error");
      break;
    case ESP32_CONNECTION_STATUS_CONNECTED:
      ret = snprintf((char*) buffer, buffer_length, "%s", "connected");
      break;
    case ESP32_CONNECTION_STATUS_TCPUDP:
      ret = snprintf((char*) buffer, buffer_length, "%s", "TCP or UDP TX created");
      break;
    case ESP32_CONNECTION_STATUS_TCPUDP_DISCONNECTED:
      ret = snprintf((char*) buffer, buffer_length, "%s", "TCP or UDP TX disconnected");
      break;
    case ESP32_CONNECTION_STATUS_NOT_CONNECTED:
      ret = snprintf((char*) buffer, buffer_length, "%s", "not connected");
      break;
  }
  return ret;
}

int esp32_get_ip_address(uint8_t* rx_buffer, uint32_t rx_buffer_length) {
  char command[] = "AT+CIFSR\r\n";
  int ret;

  ret = at_commands_txrx_sync((uint8_t*) command,
                              strlen(command),
                              strlen(command),
                              rx_buffer,
                              rx_buffer_length);
  if (ret < 0) return ret;

  // Now actually get the address into the rx_buffer.
  int address_start = 0;
  int address_end   = 0;
  for (int i = 0; i < ret; i++) {
    if (address_start == 0) {
      // If we haven't yet found the start of the address we need to do that
      // first.
      if (rx_buffer[i + 0] == 'S' &&
          rx_buffer[i + 1] == 'T' &&
          rx_buffer[i + 2] == 'A' &&
          rx_buffer[i + 3] == 'I' &&
          rx_buffer[i + 4] == 'P'
          ) {
        address_start = i + 7;
        i = address_start;
      }
    } else {
      // If we have found address_start then we need to switch to finding the
      // end of the address.
      if (rx_buffer[i] == '\"') {
        address_end = i;
        break;
      }
    }
  }

  // Check that everything worked.
  if (address_start == 0 || address_end == 0 || address_end < address_start) {
    return TOCK_FAIL;
  }

  // Copy the address into the base of the array.
  for (int i = address_start; i < address_end; i++) {
    rx_buffer[i - address_start] = rx_buffer[i];
  }
  rx_buffer[address_end - address_start] = 0;

  return address_end - address_start;
}

int esp32_dns_resolve(char* hostname, uint8_t* out_buffer, uint32_t out_buffer_length) {
  char tx_buffer[128];
  uint8_t rx_buffer[128];
  int ret;
  uint32_t index = 0;

  int length = snprintf(tx_buffer, 128, "AT+CIPDOMAIN=\"%s\"\r\n", hostname);
  ret = at_commands_txrx_sync((uint8_t*) tx_buffer,
                              128,
                              length,
                              rx_buffer,
                              128);
  if (ret < 0) return ret;
  index += ret;

  // Check if we got OK or FAIL after asking for the connection.
  if (_esp32_is_error(rx_buffer, 128) == true) return TOCK_FAIL;

  // Otherwise we need to loop and receive until we get OK or FAIL.
  while (_esp32_is_ok(rx_buffer, 128) != true) {
    // Do another receive.
    ret = at_commands_rx_sync(rx_buffer + index, 128 - index);
    if (ret < 0) return ret;
    index += ret;
    if (index >= 128) return TOCK_FAIL;

    // Now check again for success or fail
    if (_esp32_is_error(rx_buffer, 128) == true) return TOCK_FAIL;
  }

  // Now actually get the IP address into the rx_buffer.
  uint32_t address_start = 0;
  uint32_t address_end   = 0;
  for (uint32_t i = 0; i < index; i++) {
    if (address_start == 0) {
      // If we haven't yet found the start of the address we need to do that
      // first.
      if (rx_buffer[i] == ':') {
        address_start = i + 1;
        i = address_start;
      }
    } else {
      // If we have found address_start then we need to switch to finding the
      // end of the address.
      if (rx_buffer[i] == '\r') {
        address_end = i;
        break;
      }
    }
  }

  // Check that everything worked.
  if (address_start == 0 || address_end == 0 || address_end < address_start) {
    return TOCK_FAIL;
  }

  // Verify we have enough room to save this IP address.
  if ((address_end - address_start) > out_buffer_length) {
    return TOCK_FAIL;
  }

  // Copy the address into the base of the array.
  for (uint32_t i = address_start; i < address_end; i++) {
    out_buffer[i - address_start] = rx_buffer[i];
  }
  out_buffer[address_end - address_start] = 0;

  return address_end - address_start;
}

int esp32_open_tcp_socket(char* ip_address, uint16_t port) {
  char tx_buffer[128];
  uint8_t rx_buffer[128];
  int ret;
  int index = 0;

  int length = snprintf(tx_buffer, 128, "AT+CIPSTART=\"TCP\",\"%s\",%i\r\n", ip_address, port);
  ret = at_commands_txrx_sync((uint8_t*) tx_buffer,
                              128,
                              length,
                              rx_buffer,
                              128);
  if (ret < 0) return ret;
  index += ret;

  // Check if we got OK or FAIL after asking for the connection.
  if (_esp32_is_error(rx_buffer, 128) == true) return TOCK_FAIL;

  // Otherwise we need to loop and receive until we get OK or FAIL.
  while (_esp32_is_ok(rx_buffer, 128) != true) {
    // Do another receive.
    ret = at_commands_rx_sync(rx_buffer + index, 128 - index);
    if (ret < 0) return ret;
    index += ret;
    if (index >= 128) return TOCK_FAIL;

    // Now check again for success or fail
    if (_esp32_is_error(rx_buffer, 128) == true) return TOCK_FAIL;
  }

  // If we haven't returned yet then we successfully opened the TCP socket.
  return TOCK_SUCCESS;
}

int esp32_close_socket(void) {
  char command[] = "AT+CIPCLOSE\r\n";
  uint8_t rx_buffer[128];
  int ret;

  ret = at_commands_txrx_sync((uint8_t*) command,
                              strlen(command),
                              strlen(command),
                              rx_buffer,
                              128);
  if (ret < 0) return ret;

  if (_esp32_is_ok(rx_buffer, 128) == true) {
    return TOCK_SUCCESS;
  } else {
    return TOCK_FAIL;
  }
}

int esp32_socket_send_data_get_response(uint8_t* message, uint32_t message_length,
                                        uint8_t* response, uint32_t response_length) {
  char tx_buffer[128];
  uint8_t rx_buffer[128];
  int ret;
  uint32_t index = 0;

  // Send a message to the ESP32 specifying that we intend to send data it
  // should send on the TCP socket is has open.
  int length = snprintf(tx_buffer, 128, "AT+CIPSEND=%li\r\n", message_length);
  ret = at_commands_txrx_sync((uint8_t*) tx_buffer,
                              128,
                              length,
                              rx_buffer,
                              128);
  if (ret < 0) return ret;

  // Actually send the data on the TCP socket.
  ret = at_commands_txrx_sync((uint8_t*) message,
                              message_length,
                              message_length,
                              response,
                              response_length);
  if (ret < 0) return ret;
  index += ret;

  // Loop until we have received a response.
  while (_esp32_is_ipd(response, index) != true) {
    // Do another receive.
    ret = at_commands_rx_sync(response + index, response_length - index);
    if (ret < 0) return ret;
    index += ret;
    if (index >= response_length) return TOCK_FAIL;
  }

  // Sample response from the ESP32:
  //
  // ```
  // Recv 64 bytes
  //
  // SEND OK
  //
  // +IPD,343:HTTP/1.1 400 Bad Request
  // Server: nginx/1.10.3 (Ubuntu)
  // Date: Sun, 09 Dec 2018 20:29:40 GMT
  // Content-Type: text/html
  // Content-Length: 182
  // Connection: close
  //
  // <html>
  // <head><title>400 Bad Request</title></head>
  // <body bgcolor="white">
  // <center><h1>400 Bad Request</h1></center>
  // <hr><center>nginx/1.10.3 (Ubuntu)</center>
  // </body>
  // </html>
  // CLOSED
  // ```

  // Find "IPD," in the response
  uint32_t length_start         = 0;
  uint32_t length_end           = 0;
  uint32_t response_recv_length = 0;
  uint32_t response_start       = 0;
  uint32_t response_end         = 0;
  for (uint32_t i = 0; i < index - 3; i++) {
    if (length_start == 0) {
      // Start by looking for "IPD,"
      if (response[i] == 'I' && response[i + 1] == 'P' && response[i + 2] == 'D' && response[i + 3] == ',') {
        length_start = i + 4;
        i = length_start;
      }
    } else {
      // Once we have found the start of the length number, find the end.
      if (response[i] == ':') {
        length_end     = i;
        response_start = i + 1;
        break;
      }
    }
  }

  // Sanity check
  if (length_start == 0 || length_end == 0 || length_start > length_end) {
    return TOCK_FAIL;
  }

  // Get the length
  response[length_end] = 0;
  response_recv_length = atoi((char*) (response + length_start));
  response_end         = response_start + response_recv_length;

  if (response_end > index) {
    return TOCK_FAIL;
  }

  // Move response to the beginning
  for (uint32_t i = response_start; i < response_end; i++) {
    response[i - response_start] = response[i];
  }

  return response_recv_length;
}

int _esp32_parse_url(const char* url,
                     bool* https,
                     uint8_t** hostname, uint8_t** path) {
  int url_len = strlen(url);
  int offset  = 0;
  int index;

  // Get http: or https:
  index = _esp32_index_of(':', (uint8_t*) url, url_len);
  if (index < 0) return TOCK_FAIL;
  *https = (index == 5);

  // Skip "http[s]://"
  offset = index + 3;
  url   += offset; url_len -= offset;

  // Get hostname, assume a simple URL for now.
  index = _esp32_index_of('/', (uint8_t*) url, url_len);
  int hostname_len = index;
  if (hostname_len == -1) {
    hostname_len = url_len;
  }
  // Copy hostname to its own buffer.
  *hostname = malloc(hostname_len + 1);
  if (*hostname == NULL) return TOCK_FAIL;
  memcpy(*hostname, url, hostname_len);
  (*hostname)[hostname_len] = 0;

  // Skip hostname
  offset = hostname_len;
  url   += offset; url_len -= offset;

  // Copy path to its own buffer.
  int path_len = url_len;
  // Need at least room for "/"
  if (path_len == 0) path_len = 1;
  *path = malloc(path_len + 1);
  if (*path == NULL) return TOCK_FAIL;
  if (path_len == 1) {
    *path[0] = '/';
  } else {
    memcpy(*path, url, path_len);
  }
  (*path)[path_len] = 0;

  return TOCK_SUCCESS;
}

int esp32_http_get(const char* url, uint8_t* rx_buffer, uint32_t rx_buffer_length) {
  int ret;

  bool https;
  uint8_t* hostname;
  uint8_t* path;

  ret = _esp32_parse_url(url, &https, &hostname, &path);
  if (ret < 0) return ret;

  // Convert hostname to IP address using DNS lookup.
  uint8_t* ip_address = malloc(64);
  if (ip_address == NULL) return TOCK_FAIL;
  ret = esp32_dns_resolve((char*) hostname, ip_address, 64);
  if (ret < 0) return ret;

  // Ok now build the request
  uint8_t* get_request = malloc(256);
  if (get_request == NULL) return TOCK_FAIL;

  snprintf((char*) get_request, 256, "GET %s HTTP/1.1\r\n"
           "Host: %s\r\n\r\n", path, hostname);

  // Open the socket to issue the request.
  ret = esp32_open_tcp_socket((char*) ip_address, 80);
  if (ret < 0) return ret;

  // Actually issue the request.
  ret = esp32_socket_send_data_get_response(get_request, strlen((char*) get_request), rx_buffer, rx_buffer_length);
  if (ret < 0) return ret;
  rx_buffer[ret] = 0;

  // And close the socket.
  ret = esp32_close_socket();
  if (ret < 0) return ret;

  // Cleanup
  free(hostname);
  free(path);
  free(ip_address);
  free(get_request);

  return ret;
}

int esp32_http_post_form_data(const char* url,
                              uint8_t* message, uint32_t message_length,
                              uint8_t* rx_buffer, uint32_t rx_buffer_length) {
  int ret;

  bool https;
  uint8_t* hostname;
  uint8_t* path;

  ret = _esp32_parse_url(url, &https, &hostname, &path);
  if (ret < 0) return ret;

  // Convert hostname to IP address using DNS lookup.
  uint8_t* ip_address = malloc(64);
  if (ip_address == NULL) return TOCK_FAIL;
  ret = esp32_dns_resolve((char*) hostname, ip_address, 64);
  if (ret < 0) return ret;

  // Ok now build the request
  uint8_t* request = malloc(256);
  if (request == NULL) return TOCK_FAIL;

  int length = snprintf((char*) request, 256, "POST %s HTTP/1.1\r\n"
                        "Host: %s\r\n"
                        "Content-Type: application/x-www-form-urlencoded\r\n"
                        "Content-Length: %li\r\n"
                        "\r\n", path, hostname, message_length);
  uint32_t remaining = 256 - length;
  if (remaining + 3 < message_length) return TOCK_FAIL;
  memcpy(request + length, message, message_length);
  request[length + message_length]     = '\r';
  request[length + message_length + 1] = '\n';
  request[length + message_length + 2] = 0;

  // Open the socket to issue the request.
  ret = esp32_open_tcp_socket((char*) ip_address, 80);
  if (ret < 0) return ret;

  // For some reason we need a short delay here.
  delay_ms(50);

  // Actually issue the request.
  ret = esp32_socket_send_data_get_response(request, strlen((char*) request), rx_buffer, rx_buffer_length);
  if (ret < 0) return ret;
  rx_buffer[ret] = 0;

  // And close the socket.
  ret = esp32_close_socket();
  if (ret < 0) return ret;

  // Cleanup
  free(hostname);
  free(path);
  free(ip_address);
  free(request);

  return ret;
}
