#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/ipc/ipc_registry_string_name.h>
#include <libtock/ipc/ipc_relay_request.h>

// Discovery
static const char* SERVICE_NAME = "Request_Service";
static uint64_t server_ipc_id   = 0;

// Communication
#define RESPONSE_BUF_LEN 10
static uint8_t response_buf[RESPONSE_BUF_LEN] = {0};

// State Tracking
typedef enum {
  START_DISCOVERY,
  WAIT_ON_DISCOVERY,
  SEND_REQUEST,
  WAIT_ON_RESPONSE,
  WAIT_ON_CANCEL,
  ERROR,
} Client_State_t;
static Client_State_t state = START_DISCOVERY;
#define DELAY_MAX 10

// Callback when discovery completes, advances state
static void ipc_discovery_complete(returncode_t ret, uint64_t ipc_id) {
  if (ret == RETURNCODE_SUCCESS) {
    uint32_t lower = (uint32_t)ipc_id;
    uint32_t upper = (uint32_t)(ipc_id >> 32);
    printf("CLIENT: Discovery complete. Succeeded! Found IPC ID: %08X%08X\n", (unsigned int)upper, (unsigned int)lower);
    server_ipc_id = ipc_id;
    state         = SEND_REQUEST;
  } else if (ret == RETURNCODE_ENODEVICE) {
    printf("CLIENT: Discovery complete. Service not found.\n");
    state = START_DISCOVERY;
  } else {
    printf("CLIENT: Discovery complete. Failed with returncode: %d\n", ret);
    state = ERROR;
  }
}

// Handle discovering the service and advancing state
static void discover_ipc_service(void) {
  // Attempt to discover
  printf("CLIENT: Discovering: \"%s\"\n", SERVICE_NAME);
  returncode_t ret = libtock_ipc_registry_string_name_discover_service(ipc_discovery_complete, (uint8_t*)SERVICE_NAME,
                                                                       strlen(SERVICE_NAME));
  if (ret != RETURNCODE_SUCCESS) {
    printf("CLIENT: Discovery failed. Returncode: %d\n", ret);
    state = ERROR;
  } else {
    printf("CLIENT: Discovery started.\n");
    state = WAIT_ON_DISCOVERY;
  }
}

// Callback when a response is received, advances state
static void ipc_response_received(returncode_t ret, size_t len) {
  if (ret == RETURNCODE_SUCCESS) {
    printf("CLIENT: Received response. Data { ");
    for (size_t i = 0; i < len && i < RESPONSE_BUF_LEN; i++) {
      printf("%02X ", response_buf[i]);
    }
    printf("}\n");
    state = SEND_REQUEST;
  } else if (ret == RETURNCODE_ESIZE) {
    printf("CLIENT: Received partial response. Response buffer was too small. Data { ");
    for (size_t i = 0; i < len && i < RESPONSE_BUF_LEN; i++) {
      printf("%02X ", response_buf[i]);
    }
    printf("}\n");
    state = SEND_REQUEST;
  } else if (ret == RETURNCODE_ECANCEL) {
    printf("CLIENT: Request was canceled. Try again.\n");
    state = SEND_REQUEST;
  } else {
    printf("CLIENT: Response failed with returncode: %d\n", ret);
    state = ERROR;
  }
}

// Handle sending a request and advancing state
static void send_ipc_request(void) {
  // Create request data
  memset(response_buf, 0, RESPONSE_BUF_LEN);
  uint8_t request_buf[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
  printf("CLIENT: Sending request with data { ");
  for (size_t i = 0; i < 6; i++) {
    printf("%02X ", request_buf[i]);
  }
  printf("}\n");

  // Send request
  returncode_t ret = libtock_ipc_relay_request_client_send_request(ipc_response_received, server_ipc_id,
                                                                   request_buf, 6,
                                                                   response_buf, RESPONSE_BUF_LEN);
  if (ret == RETURNCODE_SUCCESS) {
    printf("CLIENT: Request initiated.\n");
    state = WAIT_ON_RESPONSE;
  } else if (ret == RETURNCODE_ENODEVICE) {
    printf("CLIENT: Server disappeared? Discover it again.\n");
    state = START_DISCOVERY;
  } else if (ret == RETURNCODE_EUNINSTALLED) {
    printf("CLIENT: Server exists, but hasn't enabled requests. Try again?\n");
    state = SEND_REQUEST;
  } else {
    printf("CLIENT: Request failed. Returncode: %d\n", ret);
    state = ERROR;
  }
}

// Handle canceling a request and advancing state
static void cancel_ipc_request(void) {
  returncode_t ret = libtock_ipc_relay_request_client_cancel_request();
  if (ret == RETURNCODE_SUCCESS) {
    printf("CLIENT: Attempting to cancel request.\n");
    state = WAIT_ON_CANCEL;
  } else {
    printf("CLIENT: Cancelling request failed. Returncode: %d\n", ret);
    state = ERROR;
  }
}

int main(void) {
  printf("CLIENT: Starting client.\n");

  // Check that we can find the driver at all
  if (!libtock_ipc_registry_string_name_exists()) {
    printf("CLIENT: IPC Registry String Name capsule does not exist on this board.\n");
    return 1;
  } else {
    printf("CLIENT: IPC Registry String Name capsule exists.\n");
  }

  // Check that we can send messages at all
  if (!libtock_ipc_relay_request_exists()) {
    printf("CLIENT: IPC Relay Request capsule does not exist on this board.\n");
    return 1;
  } else {
    printf("CLIENT: IPC Relay Request capsule exists.\n");
  }

  // Communication state machine
  uint8_t delay_counter = 0;
  while (1) {
    if (state == START_DISCOVERY) {
      discover_ipc_service();
      delay_counter = 0;
    } else if (state == WAIT_ON_DISCOVERY) {
      delay_counter++;
      if (delay_counter >= DELAY_MAX) {
        // This should never occur since discovery doesn't wait on other processes
        printf("CLIENT: Timeout waiting on discovery.\n");
        delay_counter = 0;
        state         = ERROR;
      }
    } else if (state == SEND_REQUEST) {
      send_ipc_request();
      delay_counter = 0;
    } else if (state == WAIT_ON_RESPONSE) {
      delay_counter++;
      printf("CLIENT: Waiting for response (%d of %d)\n", delay_counter, DELAY_MAX);
      if (delay_counter >= DELAY_MAX) {
        printf("CLIENT: Timeout waiting on response.\n");
        cancel_ipc_request();
        delay_counter = 0;
      }
    } else if (state == WAIT_ON_CANCEL) {
      delay_counter++;
      if (delay_counter >= DELAY_MAX) {
        // This should never occur since canceling doesn't wait on other processes
        printf("CLIENT: Timeout waiting on cancel.\n");
        delay_counter = 0;
        state         = ERROR;
      }
    } else {
      // error state
      printf("CLIENT: Stopping due to error.\n");
      return 1;
    }

    // Delay before advancing state machine
    libtocksync_alarm_delay_ms(2000);
  }
}
