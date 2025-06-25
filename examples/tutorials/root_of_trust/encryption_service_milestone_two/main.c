// Hardware Root of Trust (HWRoT) Demo Encryption Service Application
//
// When selected by the main screen HWRoT Demo application, listens for user-provided
// plaintexts over UART and encrypts them, logging status over IPC back to the screen
// application.

#include "libtock/tock.h"
#include <stdio.h>
#include <stdlib.h>

#include <libtock-sync/interface/console.h>
#include <libtock/kernel/ipc.h>

#define LOG_WIDTH      32

bool started          = false;
bool log_done         = false;
size_t screen_service = -1;
char log_buf[LOG_WIDTH] __attribute__((aligned(LOG_WIDTH)));

const char SCREEN_SERVICE_NAME[] = "org.tockos.tutorials.attestation.screen";

static void ipc_callback(__attribute__ ((unused)) int   pid,
                         __attribute__ ((unused)) int   len,
                         __attribute__ ((unused)) int   arg2,
                         __attribute__ ((unused)) void* ud) {
  started = true;
}

static void log_done_callback(__attribute__ ((unused)) int   pid,
                              __attribute__ ((unused)) int   len,
                              __attribute__ ((unused)) int   arg2,
                              __attribute__ ((unused)) void* ud) {
  log_done = true;
}

static void wait_for_start(void) {
  // Register an IPC callback and wait for it to be called by the
  // screen app based on the user's app selection.
  ipc_register_service_callback("org.tockos.tutorials.attestation.encryption", ipc_callback,
                                NULL);
  yield_for(&started);
}

static int setup_logging() {
  returncode_t ret;

  // Find the PID of the screen logging service
  ret = ipc_discover(SCREEN_SERVICE_NAME, &screen_service);
  if (ret != RETURNCODE_SUCCESS) {
    printf("Screen logging service not found.\n");
    return ret;
  }

  // Set up a callback and share so we can supply log messages
  // and know when they've been completely logged.
  ipc_register_client_callback(screen_service, log_done_callback, NULL);
  ipc_share(screen_service, log_buf, LOG_WIDTH);

  return 0;
}

static int log_to_screen(const char* message) {
  returncode_t ret;

  // Copy up to the log buffer's size of the message, with room for a null byte.
  uint16_t len = strnlen(message, sizeof(log_buf) - 1);
  memcpy(log_buf, message, len);

  // Add the null byte.
  log_buf[len] = '\0';

  // Start the logging process.
  ret = ipc_notify_service(screen_service);
  if (ret != RETURNCODE_SUCCESS) {
    printf("Failed to request a log to screen.\n");
    return ret;
  }

  // Wait for the log to complete.
  yield_for(&log_done);
  log_done = false;

  return 0;
}

static size_t request_plaintext(uint8_t* plaintext, size_t size) {
  char c;
  int number_read, number_written;

  printf("Enter plaintext to encrypt:\n");

  // Clear all leading whitespace left in the buffer.
  do {
    libtocksync_console_read((uint8_t*)&c, 1, &number_read);
  } while (c == '\n' || c == '\r');

  for (uint8_t i = 0; i < size; i++) {
    // Break on enter.
    if (c == '\n' || c == '\r') {
      libtocksync_console_write((uint8_t*)"\n", 1, &number_written);
      return i;
    }

    // Otherwise, echo the character.
    libtocksync_console_write((uint8_t*)&c, 1, &number_written);

    // Record the output.
    plaintext[i] = c;

    // Fetch a new character from input to add.
    libtocksync_console_read((uint8_t*)&c, 1, &number_read);
  }

  return size;
}

static void bytes_to_hex(char* hex_chars, uint8_t* bytes, size_t bytes_len) {
  char hex_byte[3];

  for (uint8_t i = 0; i < bytes_len; i++) {
    // Convert the current byte into hex
    sprintf(hex_byte, "%02X", bytes[i]);

    // Load that serialized byte into the destination string.
    hex_chars[2 * i]     = hex_byte[0];
    hex_chars[2 * i + 1] = hex_byte[1];
  }

  // Add a null-terminator.
  hex_chars[2 * bytes_len] = '\0';
}

int main(void) {
  returncode_t ret;

  // Wait to receive the signal to start from the app selector.
  wait_for_start();

  // Set up logging service.
  ret = setup_logging();
  if (ret < 0) {
    printf("ERROR: cannot set up logging\n");
  }

  // Prepare buffers for receiving plaintext
  uint8_t plaintext[512];
  char plaintext_hex[1024];

  while (1) {
    // Request a plaintext.
    ret = log_to_screen("Requesting plaintext...");
    if (ret < 0) {
      printf("ERROR: cannot log to screen\n");
    }
    size_t plaintext_len = request_plaintext(plaintext, sizeof(plaintext));

    // Convert first 16 bytes of plaintext to hex.
    bytes_to_hex(plaintext_hex, plaintext, plaintext_len);

    // Display the resulting hex.
    ret = log_to_screen("Returning plaintext...");
    if (ret < 0) {
      printf("ERROR: cannot log to screen\n");
    }
    printf("Received plaintext: %s\n", plaintext_hex);
  }

  return 0;
}
