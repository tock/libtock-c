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
  void*                            buffer,
  size_t                           size) {
  // Each slice's header is 8 bytes long, and we create two slices from this
  // buffer. Thus ensure that the provided buffer is at least 16 bytes long:
  if (size < 16) {
    return RETURNCODE_ESIZE;
  }

  state->driver = driver;
  state->allow  = allow;

  // We split the buffer in half, an application and kernel side. These two
  // buffers are then atomically swapped with each other.
  //
  // Initially, the first half of this buffer is designated as the application
  // buffer.
  state->app_buffer_ptr  = buffer;
  state->app_buffer_size = size / 2;

  // Write a streaming process slice header to the second half of this buffer,
  // and allow it to be the kernel buffer. We currently only support version
  // 0, and don't set the `halt` flag:
  uint8_t* kernel_buffer_ptr = state->app_buffer_ptr + state->app_buffer_size;
  size_t kernel_buffer_size  = size - state->app_buffer_size;
  streaming_process_slice_prepare_header(kernel_buffer_ptr);

  allow_rw_return_t allow_res =
    allow_readwrite(driver, allow, kernel_buffer_ptr, kernel_buffer_size);
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
  uint8_t* ret_buffer;
  uint32_t ret_size;
  bool ret_exceeded;

  // Prepare the current app buffer to be shared with the kernel (writing a
  // zeroed-out header):
  streaming_process_slice_prepare_header(state->app_buffer_ptr);

  // Swap the current app buffer for the kernel buffer:
  allow_rw_return_t allow_res =
    allow_readwrite(state->driver, state->allow, state->app_buffer_ptr,
                    state->app_buffer_size);

  if (allow_res.success) {
    // Record the new app buffer:
    state->app_buffer_ptr  = allow_res.ptr;
    state->app_buffer_size = allow_res.size;

    // Return information about the received payload:
    ret_buffer = state->app_buffer_ptr + 8;
    memcpy(&ret_size, state->app_buffer_ptr + 4, sizeof(uint32_t));
    ret_exceeded = (state->app_buffer_ptr[3] & 0x01) == 0x01;
  } else {
    // Allow was not successful, return safe dummy values instead:
    ret_buffer   = NULL;
    ret_size     = 0;
    ret_exceeded = false;
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
  uint8_t**                        buffer,
  size_t*                          size) {
  uint8_t* ret_buffer;
  size_t ret_size;

  // Unallow the buffer currently allowed to the kernel:
  allow_rw_return_t unallow_res =
    allow_readwrite(state->driver, state->allow, NULL, 0);

  if (unallow_res.success) {
    // Unallow failed, don't modify the state struct.
    ret_buffer = NULL;
    ret_size   = 0;
  } else {
    // The unallow worked, recreate the full, initial buffer from the app and
    // kernel halves:
    if ((void*)state->app_buffer_ptr < unallow_res.ptr) {
      // App buffer is left half, kernel buffer is right half:
      // `[ app_buffer ][ kernel_buffer ]`
      ret_buffer = state->app_buffer_ptr;
      ret_size   = state->app_buffer_size + unallow_res.size;
    } else {
      // App buffer is right half, kernel buffer is left half:
      // `[ kernel_buffer ][ app_buffer ]`
      ret_buffer = unallow_res.ptr;
      ret_size   = unallow_res.size + state->app_buffer_size;
    }

    // Wipe the state struct:
    memset(state, 0, sizeof(streaming_process_slice_state_t));
  }

  if (buffer != NULL) {
    *buffer = ret_buffer;
  }
  if (size != NULL) {
    *size = ret_size;
  }

  return tock_status_to_returncode(unallow_res.status);
}
