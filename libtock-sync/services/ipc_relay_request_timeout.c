#include <libtock-sync/ipc/ipc_relay_request.h>
#include <libtock-sync/ipc/syscalls/ipc_relay_request_syscalls.h>
#include <libtock/defer.h>
#include <libtock/services/alarm.h>

#include "ipc_relay_request_timeout.h"

// timeout state
static libtock_alarm_t alarm_handle;
static volatile bool cb_fired = false;
static volatile returncode_t cb_result = RETURNCODE_SUCCESS;
static volatile size_t cb_response_len = 0;

// callback for client, response received
static void client_response_received(int statuscode, int len,
                                     __attribute__ ((unused)) int unused,
                                     __attribute__ ((unused)) void* opaque) {
  // got a response. Save results
  cb_fired        = true;
  cb_result       = tock_status_to_returncode(statuscode);
  cb_response_len = (size_t)((uint32_t)len);
}

// callback for timer expiring
static void timeout_callback(__attribute__ ((unused)) uint32_t now,
                             __attribute__ ((unused)) uint32_t scheduled,
                             __attribute__ ((unused)) void*    opaque) {
  // timeout occurred, attempt to cancel transaction. This will generate an
  // upcall to get us out of the yield anyways
  libtock_ipc_relay_request_client_cancel_request();
}


returncode_t libtocksync_ipc_relay_request_client_send_request_with_timeout(
  uint64_t ipc_id,
  uint8_t* request_buf, size_t request_len,
  uint8_t* response_buf, size_t* response_len,
  uint32_t timeout_ms) {
  returncode_t err = RETURNCODE_SUCCESS;

  err = libtock_ipc_relay_request_set_allow_read_buffer(request_buf, request_len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_ipc_relay_request_set_allow_read_buffer(NULL, 0);
  }

  err = libtock_ipc_relay_request_set_allow_write_buffer(response_buf, *response_len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_ipc_relay_request_set_allow_write_buffer(NULL, 0);
  }

  if (timeout_ms != 0) {
    // timeout case. We need to manually set up callback, yield, and handle
    // outcomes

    // setup state
    cb_fired        = false;
    cb_result       = RETURNCODE_SUCCESS;
    cb_response_len = 0;

    // need to register for response_received upcalls for possible timeout
    err = libtock_ipc_relay_request_set_upcall_client_response_received(client_response_received, NULL);
    if (err != RETURNCODE_SUCCESS) return err;
    defer { libtock_ipc_relay_request_set_upcall_client_response_received(NULL, NULL);
    }

    // start a timeout countdown
    err = libtock_alarm_in_ms(timeout_ms, timeout_callback, NULL, &alarm_handle);
    if (err != RETURNCODE_SUCCESS) return err;
    defer { libtock_alarm_ms_cancel(&alarm_handle);
    }

    // actually start the request
    err = libtock_ipc_relay_request_command_client_send_request(ipc_id);
    if (err != RETURNCODE_SUCCESS) return err;

    // we're waiting on one of two possible upcalls her: Alarm or IPC Relay
    // Request, so we can't do a yield-waitfor. Instead we register both callbacks
    // and yield until one of them occurs
    while (!cb_fired) {
      yield();
    }

    // handle outcome
    if (cb_result == RETURNCODE_ECANCEL) {
      // this was a timeout. That's the only way for the request to be canceled
      return RETURNCODE_ENOACK;
    } else if (cb_result == RETURNCODE_SUCCESS) {
      // this was a successful response
      *response_len = cb_response_len;
      return RETURNCODE_SUCCESS;
    } else {
      // this was a different error
      return err;
    }
  } else {
    // no timeout case, much simpler

    err = libtock_ipc_relay_request_command_client_send_request(ipc_id);
    if (err != RETURNCODE_SUCCESS) return err;

    // no timeout, so we can do a yield-waitfor
    err = libtocksync_ipc_relay_request_yield_wait_for_client_response_received(response_len);
    if (err != RETURNCODE_SUCCESS) return err;
    return RETURNCODE_SUCCESS;
  }
}
