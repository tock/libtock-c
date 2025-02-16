#include "../tock.h"

typedef struct {
  uint8_t* app_buffer_ptr;
  size_t app_buffer_size;
  uint32_t driver;
  uint32_t allow;
} streaming_process_slice_state_t;

// Initialize a "streaming process slice" read-write allow slot
//
// This method allows a userspace buffer into a "streaming process
// slice" allow slot, implementing its atomic-swap semantics and
// header layout. The streaming process slice abstraction allows a
// userspace process to lossessly receive data from a kernel
// capsule. This is done by maintaining two buffers, where at any time
// one of which is owned by the kernel (for writing new, incoming data
// into) and one by the application, to process received data. These
// buffers are atomically swapped by the application, upon receipt of
// a signal that some data has been inserted into the kernel-owned
// buffer (such as an upcall).
//
// This method abstracts this interface by consuming one buffer and
// splitting it into two halves, owned by the application and kernel
// respectively. It tracks all necessary state in the
// `streaming_process_slice_state_t` object. For this to work, the
// passed buffer must be able to hold at least two streaming process
// slice headers (8 byte each), i.e., it must be at least 16 bytes
// long.
//
// In case of an error while allowing the kernel-owned buffer to the
// specified driver and read-write allow slot, this function converts
// this error-status into a returncode using
// `tock_status_to_returncode` and returns it to the caller. When this
// method returns `RETURNCODE_SUCCESS`, the passed buffer is assumed
// to be owned by this `streaming_process_slice_state_t` and must not
// be used until after a successful call to
// `streaming_process_slice_deinit`. When the buffer is of
// insufficient size, it returns `RETURNCODE_ESIZE`.
returncode_t streaming_process_slice_init(
  streaming_process_slice_state_t* state,
  uint32_t                         driver,
  uint32_t                         allow,
  void*                            buffer,
  size_t                           size);

// Swap kernel- for app-owned buffer and get received payload
//
// This method atomically swaps the kernel-owned and application-owned
// halves of the streaming process slice. This function will reset the
// application-owned buffers header, applying any flags set in the
// `streaming_process_slice_state_t` and setting the write offset to
// `0`.
//
// Following the swap operation, when returning `RETURNCODE_SUCCESS`,
// it provides the buffer's payload and any kernel-set flags to the
// caller through the `buffer`, `size`, and `exceeded` arguments
// respectively. Callers must either provide pointers to variables for
// these values, or set them to `NULL` in case they are not interested
// in any given value.
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
// This function reconstructs the passed into `streaming_process_slice_init` and
// returns it through the `buffer` and `size` arguments (if not set to `NULL`
// respectively).
//
// This function forwards any error from the underlying `allow_readwrite`
// operation in its return value. In case of a return value other than
// `RETURNCODE_SUCCESS`, any values returned in `buffer`, `size` and `exceeded`
// must not be considered valid.
returncode_t streaming_process_slice_deinit(
  streaming_process_slice_state_t* state,
  uint8_t**                        buffer,
  size_t*                          size);
