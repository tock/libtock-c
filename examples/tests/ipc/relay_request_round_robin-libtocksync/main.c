#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/ipc/ipc_registry_string_name.h>
#include <libtock-sync/ipc/ipc_relay_request.h>
#include <libtock-sync/services/alarm.h>
#include <libtock-sync/services/ipc_relay_request_timeout.h>


// Testing setup. Run `make ROUNDROBIN` to automatically build all three
// services and upload them to the board
#ifndef SERVICE
#define SERVICE 0
#endif

#if SERVICE == 0
// First service
static char* SERVICE_NAME = (char*)"Service0";
static char* PRIOR_NAME   = (char*)"Service2";
static char* NEXT_NAME    = (char*)"Service1";
bool send_first = false;
#elif SERVICE == 1
// Second service
static char* SERVICE_NAME = (char*)"Service1";
static char* PRIOR_NAME   = (char*)"Service0";
static char* NEXT_NAME    = (char*)"Service2";
bool send_first = false;
#else
// Third service, note that this one sends first to break the deadlock
static char* SERVICE_NAME = (char*)"Service2";
static char* PRIOR_NAME   = (char*)"Service1";
static char* NEXT_NAME    = (char*)"Service0";
bool send_first = true;
#endif


// Discovery
static uint64_t prior_ipc_id = 0;
static uint64_t next_ipc_id  = 0;

// Communication
static uint8_t value = 0;

static returncode_t discover_ipc_service(uint8_t* name, size_t name_len, uint64_t* ipc_id) {
  while (true) {
    returncode_t ret = libtocksync_ipc_registry_string_name_discover_service(name, name_len, ipc_id);
    if (ret == RETURNCODE_SUCCESS) {
      // Success! Stop searching
      uint32_t upper = (uint32_t)((*ipc_id) >> 32);
      uint32_t lower = (uint32_t)(*ipc_id);
      printf("%s: IPC String Name discovered service \"%s\". IPC ID: %08X%08X\n", SERVICE_NAME, name,
             (unsigned int)upper, (unsigned int)lower);
      break;
    } else if (ret == RETURNCODE_ENODEVICE) {
      // Didn't discover it (yet, it may still register later)
      printf("%s: Couldn't find service \"%s\" yet.\n", SERVICE_NAME, name);
      libtocksync_alarm_delay_ms(1000); // wait before trying again
    } else {
      // An error occurred
      printf("%s: IPC String Name discovery had an error. Returncode: %d\n", SERVICE_NAME, ret);
      return ret;
    }
  }
  return RETURNCODE_SUCCESS;
}

static returncode_t handle_discovery(void) {
  // Discover prior service
  returncode_t ret = discover_ipc_service((uint8_t*)PRIOR_NAME, strlen(PRIOR_NAME), &prior_ipc_id);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Discover next service
  ret = discover_ipc_service((uint8_t*)NEXT_NAME, strlen(NEXT_NAME), &next_ipc_id);
  if (ret != RETURNCODE_SUCCESS) return ret;

  return RETURNCODE_SUCCESS;
}

static returncode_t send_ipc_request(void) {
  // Send request to next process
  uint8_t request_buf[1]  = {0};
  size_t request_len      = 1;
  uint8_t response_buf[1] = {0};
  size_t response_len     = 1;
  uint32_t timeout_ms     = 1000;
  request_buf[0] = value + 1; // increment value
  returncode_t ret = libtocksync_ipc_relay_request_client_send_request_with_timeout(next_ipc_id, request_buf,
                                                                                    request_len, response_buf,
                                                                                    &response_len, timeout_ms);
  if (ret != RETURNCODE_SUCCESS) {
    // Error case
    printf("%s: Error sending request. Returncode: %d\n", SERVICE_NAME, ret);
    return ret;
  }

  // Got proper response! Update value to match
  value = response_buf[0];
  uint32_t upper = (uint32_t)(next_ipc_id >> 32);
  uint32_t lower = (uint32_t)next_ipc_id;
  printf("%s: Got response from %08X%08X! Value is now: %d\n", SERVICE_NAME, (unsigned int)upper, (unsigned int)lower,
         value);
  return RETURNCODE_SUCCESS;
}

static returncode_t receive_ipc_request(void) {
  // Receive request from prior process
  uint8_t retry_count    = 0;
  uint8_t request_buf[1] = {0};
  size_t request_len     = 1;
  while (true) {
    request_len = 1;
    returncode_t ret = libtocksync_ipc_relay_request_server_get_next_request_from(prior_ipc_id, request_buf,
                                                                                  &request_len);
    if (ret == RETURNCODE_SUCCESS) {
      // Successful request
      break;
    } else if (ret == RETURNCODE_ENODEVICE) {
      // No request right now, try again in a bit
      // uint32_t upper = (uint32_t)(prior_ipc_id >> 32);
      // uint32_t lower = (uint32_t)prior_ipc_id;
      // printf("%s: No request yet from: %08X%08X.\n", SERVICE_NAME, (unsigned int)upper, (unsigned int)lower);
      retry_count++;
      if (retry_count >= 50) {
        // Timeout on waiting for request
        return RETURNCODE_ENODEVICE;
      }
      libtocksync_alarm_delay_ms(100);
      continue;
    } else {
      // An error occurred
      printf("%s: Error getting request. Returncode: %d\n", SERVICE_NAME, ret);
      return ret;
    }
  }

  // Got request, send response echoing new value
  value = request_buf[0];
  uint8_t response_buf[1] = {value};
  size_t response_len     = 1;
  returncode_t ret        = libtocksync_ipc_relay_request_server_send_response(response_buf, response_len);
  if (ret != RETURNCODE_SUCCESS) {
    printf("%s: Error sending response. Returncode: %d\n", SERVICE_NAME, ret);
    return ret;
  }
  return RETURNCODE_SUCCESS;
}

int main(void) {
  returncode_t ret = RETURNCODE_SUCCESS;
  printf("%s: Starting.\n", SERVICE_NAME);

  // Check that we can find the driver at all
  if (!libtock_ipc_registry_string_name_exists()) {
    printf("%s: IPC Registry String Name capsule does not exist on this board.\n", SERVICE_NAME);
    return 1;
  } else {
    printf("%s: IPC Registry String Name capsule exists.\n", SERVICE_NAME);
  }

  // Check that we can send messages at all
  if (!libtock_ipc_relay_request_exists()) {
    printf("%s: IPC Relay Request capsule does not exist on this board.\n", SERVICE_NAME);
    return 1;
  } else {
    printf("%s: IPC Relay Request capsule exists.\n", SERVICE_NAME);
  }

  // Enable requests. Importantly, do this BEFORE registering so that you can
  // immediately receive requests after being discovered
  libtocksync_ipc_relay_request_server_enable_requests();

  // Register service name
  ret = libtocksync_ipc_registry_string_name_register_service((uint8_t*)SERVICE_NAME, strlen(SERVICE_NAME));
  if (ret != RETURNCODE_SUCCESS) {
    printf("%s: IPC String Name registration failed. Returncode: %d\n", SERVICE_NAME, ret);
    return 1;
  } else {
    printf("%s: IPC String Name registered.\n", SERVICE_NAME);
  }

  // Discovery the other two services
  ret = handle_discovery();
  if (ret != RETURNCODE_SUCCESS) return 1;

  // Send and receive forever
  bool sending = send_first;
  while (true) {
    if (sending) {
      // Sending request to next service
      ret = send_ipc_request();
      if (ret == RETURNCODE_SUCCESS) {
        // Success, move to next state
        sending = !sending;
      } else if (ret == RETURNCODE_ENODEVICE) {
        // Server doesn't exist anymore. Try discovery again
        printf("%s: Server disappeared? Discovering again.\n", SERVICE_NAME);
        ret = handle_discovery();
        if (ret != RETURNCODE_SUCCESS) return ret;
      } else if (ret == RETURNCODE_ENOACK) {
        // Timeout. Stay in same state to just try sending again?
        printf("%s: timeout on request. We'll try again...\n", SERVICE_NAME);
      } else {
        // Error
        return 1;
      }
    } else {
      // Get request from prior process and send response
      ret = receive_ipc_request();
      if (ret == RETURNCODE_SUCCESS) {
        // Success, move to next state
        sending = !sending;
      } else if (ret == RETURNCODE_EINVAL) {
        // Couldn't send response? There was no transaction to respond to. This
        // means the client canceled or faulted. It's not an error, so just try
        // receiving again.
        printf("%s: Client disappeared? Listening for request again.\n", SERVICE_NAME);
      } else if (ret == RETURNCODE_ENODEVICE) {
        // timed out trying to find a request from the prior service. Do
        // discovery again
        printf("%s: Never heard from client? Discovering again.\n", SERVICE_NAME);
        ret = handle_discovery();
        if (ret != RETURNCODE_SUCCESS) return ret;
      } else {
        // Error
        return 1;
      }
    }

    // Delay a bit
    libtocksync_alarm_delay_ms(1000);
  }
}
