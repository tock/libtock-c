#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/ipc/ipc_registry_string_name.h>
#include <libtock/ipc/ipc_relay_request.h>

// Registration
static const char* SERVICE_NAME = "Request_Service";
static bool service_registered  = false;

// Callback when registration completes
static void ipc_registration_complete(returncode_t ret) {
  if (ret == RETURNCODE_SUCCESS) {
    printf("SERVER: Registration complete. Succeeded!\n");
    service_registered = true;
  } else {
    printf("SERVER: Registration complete. Failed with returncode: %d\n", ret);
  }
}

// Handle all existing requests. Returns when no outstanding requests remain.
static void handle_requests(void) {
  // loop until all requests are completed
  while (true) {

    // Attempt to get a request
    uint64_t request_ipc_id = 0;
    uint8_t request_buf[10] = {0};
    size_t request_len      = 10;
    returncode_t ret        = libtock_ipc_relay_request_server_get_next_request(&request_ipc_id, request_buf,
                                                                                &request_len);

    if (ret == RETURNCODE_SUCCESS) {
      // Request successfully received
      uint32_t lower = (uint32_t)request_ipc_id;
      uint32_t upper = (uint32_t)(request_ipc_id >> 32);
      printf("SERVER: Got a request from IPC ID: %08X%08X\n", (unsigned int)upper, (unsigned int)lower);
      printf("SERVER: Request data {");
      for (size_t i = 0; i < request_len; i++) {
        printf("%02X ", request_buf[i]);
      }
      printf("}\n");

      // Delay for a while to simulate a server hard at work on a request
      libtocksync_alarm_delay_ms(6000);

      // Send response
      uint8_t response_buf[4] = {0x12, 0x34, 0x56, 0x78};
      ret = libtock_ipc_relay_request_server_send_response(response_buf, 4);
      if (ret != RETURNCODE_SUCCESS) {
        // The best course of action is to ignore the failure here. Most
        // failures mean that either the client no longer exists, that it
        // canceled its request, or that it didn't properly allow buffers.
        printf("SERVER: Sending response failed with returncode: %d The server can ignore this.\n", ret);
      } else {
        printf("SERVER: Sent response.\n");
      }
    } else if (ret == RETURNCODE_ESIZE) {
      // Request partially received. Request buffer was too small.
      uint32_t lower = (uint32_t)request_ipc_id;
      uint32_t upper = (uint32_t)(request_ipc_id >> 32);
      printf("SERVER: Got a PARTIAL request from IPC ID: %08X%08X\n", (unsigned int)upper, (unsigned int)lower);
      printf("SERVER: Request data (truncated) {");
      for (size_t i = 0; i < request_len; i++) {
        printf("%02X ", request_buf[i]);
      }
      printf("}\n");

      // Send response
      uint8_t response_buf[1] = {0xFF};
      ret = libtock_ipc_relay_request_server_send_response(response_buf, 1);
      if (ret != RETURNCODE_SUCCESS) {
        // The best course of action is to ignore the failure here. Most
        // failures mean that either the client no longer exists, that it
        // canceled its request, or that it didn't properly allow buffers.
        printf("SERVER: Sending response failed with returncode: %d The server can ignore this.\n", ret);
      } else {
        printf("SERVER: Sent response.\n");
      }
    } else if (ret == RETURNCODE_ENODEVICE) {
      // No request was waiting. This is not an error. It instead means we
      // exhausted all requests.
      break;
    } else {
      // Some other error? This shouldn't happen.
      printf("SERVER: Error when getting request with returncode: %d\n", ret);
      break;
    }
  }
}

// Callback when a request is waiting for us
static void ipc_request_waiting(uint64_t ipc_id) {
  uint32_t lower = (uint32_t)ipc_id;
  uint32_t upper = (uint32_t)(ipc_id >> 32);
  printf("SERVER: New request waiting from IPC ID: %08X%08X\n", (unsigned int)upper, (unsigned int)lower);

  // Handle request(s). Loops until no further requests remain.
  handle_requests();
}

int main(void) {
  returncode_t ret = RETURNCODE_SUCCESS;
  printf("SERVER: Starting server.\n");

  // Check that we can find the driver at all
  if (!libtock_ipc_registry_string_name_exists()) {
    printf("SERVER: IPC Registry String Name capsule does not exist on this board.\n");
    return 1;
  } else {
    printf("SERVER: IPC Registry String Name capsule exists.\n");
  }

  // Check that we can send messages at all
  if (!libtock_ipc_relay_request_exists()) {
    printf("SERVER: IPC Relay Request capsule does not exist on this board.\n");
    return 1;
  } else {
    printf("SERVER: IPC Relay Request capsule exists.\n");
  }

  // Enable requests and callbacks when a new request is waiting. Importantly,
  // do this BEFORE registering as a service, that way you're ready to receive
  // requests the moment you are discovered.
  ret = libtock_ipc_relay_request_server_enable_requests_with_callback(ipc_request_waiting);
  if (ret != RETURNCODE_SUCCESS) {
    printf("SERVER: Enabling request waiting callbacks failed. Returncode: %d\n", ret);
    return 1;
  } else {
    printf("SERVER: Enabled request waiting callbacks.\n");
  }

  // Register service
  printf("SERVER: Registering with string name: \"%s\"\n", SERVICE_NAME);
  ret = libtock_ipc_registry_string_name_register_service(ipc_registration_complete, (uint8_t*)SERVICE_NAME,
                                                          strlen(SERVICE_NAME));
  if (ret != RETURNCODE_SUCCESS) {
    printf("SERVER: Registration failed. Returncode: %d\n", ret);
    return 1;
  } else {
    printf("SERVER: Registration started.\n");
  }

  // Delay a bit
  libtocksync_alarm_delay_ms(2000);

  // Timeout. Registration shouldn't fail
  if (!service_registered) {
    printf("SERVER: Registration didn't succeed. Giving up.\n");
    return 1;
  }

  // Request handling will be done in the callback. Just yield forever
  while (true) {
    yield();
  }
}
