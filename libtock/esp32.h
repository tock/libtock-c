#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

// Query the ESP32 Wi-Fi chip for information about its version and compile
// time.
//
// Return value is <0 on error, and the length of the version information on
// success. The version information will be at the beginning of `rx_buffer`.
int esp32_get_chip_version(uint8_t* rx_buffer, uint32_t rx_buffer_length);

// Get the MAC address of the Wi-Fi client as a string.
//
// Return value is <0 on error, and `TOCK_SUCCESS` on success. The MAC address
// will be a string at the beginning of `rx_buffer`.
int esp32_get_station_mac_address(uint8_t* rx_buffer, uint32_t rx_buffer_length);

// Do a factory reset of the chip and clear all saved values in the nonvolatile
// memory on the chip.
//
// This will return `TOCK_SUCCESS` on success, and `TOCK_FAIL` if the chip did
// not succeed when trying to do a factory reset. It will also return an error
// code if an issue was encountered in the kernel when trying to send the AT
// command.
int esp32_factory_reset(void);

// Do a scan for nearby Wi-Fi access points and return the results as a string.
//
// Note: it is important to have a large enough buffer here in case there are
// many nearby networks.
//
// Return value is <0 on error, and the length of the returned string on
// success.
int esp32_get_aps(uint8_t* rx_buffer, uint32_t rx_buffer_length);

typedef enum {
	ESP32_MODE_UNKNOWN = 0,
	ESP32_MODE_STATION = 1,
	ESP32_MODE_SOFT_AP = 2,
	ESP32_MODE_STATION_SOFT_AP = 3,
} esp32_mode_e;

// Get the current operating mode of the ESP32. This indicates if the chip is
// acting as a client (station) or access point (soft AP).
//
// Return value is <0 on error, and `esp32_mode_e` on success.
//
// Example usage:
//
// ```c
// ret = esp32_get_mode();
// if (ret < 0) {
//   printf("get_mode failed %i\n", ret);
//   return -1;
// }
// ret = esp32_mode2str(ret, recv_buffer, BUFFER_LEN);
// if (ret <= 0) {
//   printf("mode2str failed %i\n", ret);
//   return -1;
// }
// printf("Mode: %s\n", recv_buffer);
// ```
int esp32_get_mode(void);

// Set the operating mode of the ESP32. This configures the chip between
// station and access point mode.
//
// Return value is <0 on error, and `TOCK_SUCCESS` on success.
int esp32_set_mode(esp32_mode_e mode);

// Convert a `esp32_mode_e` to a human readable string.
//
// Return value is the length of the string in the buffer.
int esp32_mode2str(esp32_mode_e mode, uint8_t* buffer, uint32_t buffer_length);

// Connect to a Wi-Fi network with the given SSID and password.
//
// Returns <0 on error, and `TOCK_SUCCESS` if the network was successfully
// joined. Returns `TOCK_FAIL` if the network could not be joined.
int esp32_connect_to_network(const char* ssid, const char* password);

typedef enum {
	ESP32_CONNECTION_STATUS_UNKNOWN = 0,
	ESP32_CONNECTION_STATUS_CONNECTED = 2,
	ESP32_CONNECTION_STATUS_TCPUDP = 3,
	ESP32_CONNECTION_STATUS_TCPUDP_DISCONNECTED = 4,
	ESP32_CONNECTION_STATUS_NOT_CONNECTED = 5,
} esp32_connection_status_e;

// Retrieve the connection status of the ESP32.
//
// Returns <0 on an error, and `esp32_connection_status_e` otherwise.
int esp32_get_connection_status(void);

// Convert a `esp32_connection_status_e` to a human readable string.
//
// Return value is the length of the string in the buffer.
int esp32_connection_status2str(esp32_connection_status_e mode, uint8_t* buffer, uint32_t buffer_length);

// Get the IP address of the station after it is connected to the access point.
//
// Returns <0 on error. On success, the return value is the length of the string
// of the IP address. If the response from the ESP32 does not contain a station
// IP address then `TOCK_FAIL` is returned.
int esp32_get_ip_address(uint8_t* rx_buffer, uint32_t rx_buffer_length);

// Do a DNS lookup to resolve a hostname to an IP address. `hostname` should be
// a c-string.
//
// The IP address will be placed in rx_buffer. The return value will be <0 if
// there was an error, otherwise it will be the string length of the IP address
// in `rx_buffer`.
int esp32_dns_resolve(char* hostname, uint8_t* rx_buffer, uint32_t rx_buffer_length);

// Establish a TCP connection with a remote server on the given IP address and
// port. `ip_address` should be a c-string.
//
// Returns <0 on error, and `TOCK_SUCCESS` if the connection was successfully
// established.
int esp32_open_tcp_socket(char* ip_address, uint16_t port);

// Close an open socket.
//
// Returns <0 if there is an error, and `TOCK_SUCCESS` if the socket was
// successfully closed.
int esp32_close_socket(void);

// Send data to an open socket and wait for an immediate response.
//
// The response will be placed in `response`. If an error occurs, the return
// value will be <0. On success, the return value will be the length of the
// received response.
int esp32_socket_send_data_get_response(uint8_t* message, uint32_t message_length,
                                        uint8_t* response, uint32_t response_length);

// Perform a very simple HTTP GET call with basic urls.
//
// The entire HTTP response will be in rx_buffer. On error, return value will be
// <0, and on success it will be the length of the received data.
int esp32_http_get(const char* url, uint8_t* rx_buffer, uint32_t rx_buffer_length);

// Perform an HTTP POST request with content type
// "application/x-www-form-urlencoded".
//
// The entire HTTP response will be in rx_buffer. On error, return value will be
// <0, and on success it will be the length of the received data.
int esp32_http_post_form_data(const char* url,
                              uint8_t* message, uint32_t message_length,
                              uint8_t* rx_buffer, uint32_t rx_buffer_length);

#ifdef __cplusplus
}
#endif
