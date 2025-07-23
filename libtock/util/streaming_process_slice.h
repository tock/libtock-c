#include "../tock.h"

#define STREAMING_PROCESS_SLICE_HEADER_LEN 8

typedef struct {
  uint32_t driver;
  uint32_t allow;
  uint8_t* app_buffer_ptr;
  size_t app_buffer_size;
  bool app_buffer_is_b;
} streaming_process_slice_state_t;

// Initialize a "streaming process slice" read-write allow slot
//
// This method allows a userspace buffer into a "streaming process slice" allow
// slot, implementing its atomic-swap semantics and header layout. The streaming
// process slice abstraction allows a userspace process to lossessly receive
// data from a kernel capsule. This is done by maintaining two buffers, where at
// any time one of which is owned by the kernel (for writing new, incoming data
// into) and one by the application, to process received data. These buffers are
// atomically swapped by the application, upon receipt of a signal that some
// data has been inserted into the kernel-owned buffer (such as an upcall).
//
// This method abstracts this interface by consuming two buffers, owned by the
// application and kernel respectively. It tracks all necessary state in the
// `streaming_process_slice_state_t` object. This struct is initialized by this
// method.
//
// The passed buffers must be each be able to hold at least the streaming
// process slice headers (`STREAMING_PROCESS_SLICE_HEADER_LEN` bytes), in
// addition to any payload.
//
// In case of an error while allowing the kernel-owned buffer to the specified
// driver and read-write allow slot, this function converts this error-status
// into a returncode using `tock_status_to_returncode` and returns it to the
// caller. When this method returns `RETURNCODE_SUCCESS`, the passed buffers are
// assumed to be owned by this `streaming_process_slice_state_t` and must not be
// used until after a successful call to `streaming_process_slice_deinit`. When
// either buffer is of insufficient size, it returns `RETURNCODE_ESIZE` and does
// not perform any allow operation or initialize the state reference.
returncode_t streaming_process_slice_init(
  streaming_process_slice_state_t* state,
  uint32_t                         driver,
  uint32_t                         allow,
  void*                            buffer_a,
  size_t                           size_a,
  void*                            buffer_b,
  size_t                           size_b);

// Swap kernel- for app-owned buffer and get received payload
//
// This method atomically swaps the kernel-owned and application-owned buffers
// backing this streaming process slice. This function will reset the
// application-owned buffers header, applying any flags set in the
// `streaming_process_slice_state_t` and setting the write offset to `0`.
//
// Following the swap operation, when returning `RETURNCODE_SUCCESS`, it
// provides the buffer's payload and any kernel-set flags to the caller through
// the `buffer`, `size`, and `exceeded` arguments respectively. Callers must
// either provide pointers to variables for these values, or set them to `NULL`
// in case they are not interested in any given value.
//
// This function forwards any error from the underlying `allow_readwrite`
// operation in its return value. In case of a return value other than
// `RETURNCODE_SUCCESS`, any values returned in `buffer`, `size` and `exceeded`
// must not be considered valid.
returncode_t streaming_process_slice_get_and_swap(
  streaming_process_slice_state_t* state,
  uint8_t**                        buffer,
  uint32_t*                        size,
  bool*                            exceeded);

// Deinitialize an initialized `streaming_process_slice_state_t`
//
// This function returns the buffers passed into `streaming_process_slice_init`
// through the `buffer_a`, `size_a`, `buffer_b` and `size_b` arguments (if not
// set to `NULL` respectively). It ensures that the `buffer_a` and `size_a`
// arguments passed to `streaming_process_slice_init` match those of `buffer_a`
// and `size_a` for this function, i.e., it will not swap `buffer_a` for
// `buffer_b`, regardless of the number of calls to
// `streaming_process_slice_get_and_swap`.
//
// This function forwards any error from the underlying `allow_readwrite`
// operation in its return value. In case of a return value other than
// `RETURNCODE_SUCCESS`, any values returned in `buffer`, `size` and `exceeded`
// must not be considered valid.
returncode_t streaming_process_slice_deinit(
  streaming_process_slice_state_t* state,
  uint8_t**                        buffer_a,
  size_t*                          size_a,
  uint8_t**                        buffer_b,
  size_t*                          size_b);
