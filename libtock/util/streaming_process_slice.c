#include "streaming_process_slice.h"
#include <string.h>

static void streaming_process_slice_prepare_header(uint8_t* buf) {
  buf[0] = 0;  // version[H]
  buf[1] = 0;  // version[L]
  buf[2] = 0;  // flags[H]
  buf[3] = 0;  // flags[L]
  buf[4] = 0;  // write offset
  buf[5] = 0;  // write offset
  buf[6] = 0;  // write offset
  buf[7] = 0;  // write offset
}

returncode_t streaming_process_slice_init(
  streaming_process_slice_state_t* state,
  uint32_t                         driver,
  uint32_t                         allow,
  void*                            buffer_a,
  size_t                           size_a,
  void*                            buffer_b,
  size_t                           size_b) {
  // Ensure that both buffers can hold the streaming process slice header:
  if (size_a < STREAMING_PROCESS_SLICE_HEADER_LEN || size_b < STREAMING_PROCESS_SLICE_HEADER_LEN) {
    return RETURNCODE_ESIZE;
  }

  state->driver = driver;
  state->allow  = allow;

  // Initially, `buffer_a` is used as the application-owned buffer.
  state->app_buffer_ptr  = buffer_a;
  state->app_buffer_size = size_a;
  state->app_buffer_is_b = false; // false -> buffer_a

  // Write a streaming process slice header to bufferB, and use it as the
  // kernel-owned buffer. Its pointer and length are stored within the kernel's
  // allow slot. We currently only support version 0, and don't set the `halt`
  // flag:
  streaming_process_slice_prepare_header(buffer_b);

  allow_rw_return_t allow_res =
    allow_readwrite(driver, allow, buffer_b, size_b);
  if (!allow_res.success) {
    memset(state, 0, sizeof(streaming_process_slice_state_t));
  }

  return tock_status_to_returncode(allow_res.status);
}

returncode_t streaming_process_slice_get_and_swap(
  streaming_process_slice_state_t* state,
  uint8_t**                        buffer,
  uint32_t*                        size,
  bool*                            exceeded) {
  // Prepare the current app buffer to be shared with the kernel (writing a
  // zeroed-out header):
  streaming_process_slice_prepare_header(state->app_buffer_ptr);

  // Swap the current app buffer for the kernel buffer:
  allow_rw_return_t allow_res =
    allow_readwrite(state->driver, state->allow, state->app_buffer_ptr,
                    state->app_buffer_size);

  // Initialize to safe dummy values in case the allow was not successful
  uint8_t* ret_buffer = NULL;
  uint32_t ret_size   = 0;
  bool ret_exceeded   = false;
  if (allow_res.success) {
    // Record the new app buffer:
    state->app_buffer_ptr  = allow_res.ptr;
    state->app_buffer_size = allow_res.size;
    state->app_buffer_is_b = !state->app_buffer_is_b;

    // Return information about the received payload:
    ret_buffer = state->app_buffer_ptr + 8;
    memcpy(&ret_size, state->app_buffer_ptr + 4, sizeof(uint32_t));
    ret_exceeded = (state->app_buffer_ptr[3] & 0x01) == 0x01;
  }

  // Write return values if provided with non-NULL pointers:
  if (buffer != NULL) {
    *buffer = ret_buffer;
  }
  if (size != NULL) {
    *size = ret_size;
  }
  if (exceeded != NULL) {
    *exceeded = ret_exceeded;
  }

  return tock_status_to_returncode(allow_res.status);
}

returncode_t streaming_process_slice_deinit(
  streaming_process_slice_state_t* state,
  uint8_t**                        buffer_a,
  size_t*                          size_a,
  uint8_t**                        buffer_b,
  size_t*                          size_b) {
  // Safe default values:
  uint8_t* ret_buffer_a = NULL;
  size_t ret_size_b     = 0;
  uint8_t* ret_buffer_b = NULL;
  size_t ret_size_a     = 0;

  // Unallow the buffer currently allowed to the kernel:
  allow_rw_return_t unallow_res =
    allow_readwrite(state->driver, state->allow, NULL, 0);

  if (unallow_res.success) {
    // The unallow worked, recreate the full, initial buffer from the app and
    // kernel halves:
    if (state->app_buffer_is_b) {
      ret_buffer_a = unallow_res.ptr;
      ret_size_a   = unallow_res.size;
      ret_buffer_b = state->app_buffer_ptr;
      ret_size_b   = state->app_buffer_size;
    } else {
      ret_buffer_a = state->app_buffer_ptr;
      ret_size_a   = state->app_buffer_size;
      ret_buffer_b = unallow_res.ptr;
      ret_size_b   = unallow_res.size;
    }

    // Wipe the state struct:
    memset(state, 0, sizeof(streaming_process_slice_state_t));
  }

  if (buffer_a != NULL) {
    *buffer_a = ret_buffer_a;
  }
  if (size_a != NULL) {
    *size_a = ret_size_a;
  }
  if (buffer_b != NULL) {
    *buffer_b = ret_buffer_b;
  }
  if (size_b != NULL) {
    *size_b = ret_size_b;
  }

  return tock_status_to_returncode(unallow_res.status);
}
