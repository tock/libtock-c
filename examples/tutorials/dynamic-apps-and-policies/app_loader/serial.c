
#include <stdbool.h>

#include <libtock/defer.h>
#include <libtock-sync/services/alarm.h>

#include "serial.h"

#define DRIVER_NUM_YMODEM 0xA0000

// Upcall number, matching `capsules_core::console::Console`'s syscall ABI
// (this driver is a second instance of that same capsule, registered under
// `DRIVER_NUM_YMODEM` instead of the usual console driver number).
#define UPCALL_READ_DONE  2





static returncode_t libtock_ymodem_set_read_allow(const uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_YMODEM, 1, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

static returncode_t libtock_ymodem_set_readwrite_allow(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_YMODEM, 1, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}



static returncode_t libtock_ymodem_command_write(int length) {
  syscall_return_t cval = command(DRIVER_NUM_YMODEM, 1, length, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

static returncode_t libtock_ymodem_command_read(int length) {
  syscall_return_t cval = command(DRIVER_NUM_YMODEM, 2, length, 0);
  return tock_command_return_novalue_to_returncode(cval);
}




static returncode_t libtocksync_ymodem_yield_wait_for_write(uint32_t* bytes_written) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_YMODEM, 1);

  *bytes_written = ret.data1;

  return tock_status_to_returncode(ret.data0);
}

static returncode_t libtocksync_ymodem_yield_wait_for_read(uint32_t* bytes_read) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_YMODEM, 2);

  *bytes_read = ret.data1;

  return tock_status_to_returncode(ret.data0);
}

bool libtock_ymodem_driver_exists(void) {
  return driver_exists(DRIVER_NUM_YMODEM);
}

returncode_t libtocksync_ymodem_write(const uint8_t* buffer, uint32_t length, uint32_t* written) {
  int err;

  err = libtock_ymodem_set_read_allow(buffer, length);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_ymodem_set_read_allow(NULL, 0);
  }

  err = libtock_ymodem_command_write((int) length);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_ymodem_yield_wait_for_write(written);
  return err;
}

returncode_t libtocksync_ymodem_read(uint8_t* buffer, uint32_t length, uint32_t* read) {
  int err;

  err = libtock_ymodem_set_readwrite_allow(buffer, length);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_ymodem_set_readwrite_allow(NULL, 0);
  }

  err = libtock_ymodem_command_read((int) length);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_ymodem_yield_wait_for_read(read);
  return err;
}

// ---- Timeout-capable read, built on the async (subscribe-based) syscall
// interface instead of `yield_wait_for` ----
//
// This driver doesn't implement `receive_abort()`, so a timed-out read
// can't be cancelled at the kernel. Instead of aborting, a timeout here
// just gives up *waiting* and returns, leaving the buffer allowed and the
// receive running; see `libtocksync_ymodem_read_with_timeout`'s doc
// comment in serial.h for the resulting calling contract.

typedef void (*libtock_ymodem_callback_read)(returncode_t status, uint32_t length);

// Raw `subscribe()` upcalls carry (status, length, unused, userdata); adapt
// that into the simpler `libtock_ymodem_callback_read` shape, matching how
// `libtock/interface/console.c` wraps the standard console driver's async
// API.
static void libtock_ymodem_read_done_trampoline(int status, int length,
                                                 __attribute__((unused)) int unused,
                                                 void* ud) {
  libtock_ymodem_callback_read cb = (libtock_ymodem_callback_read) ud;
  cb(tock_status_to_returncode(status), (uint32_t) length);
}

// Async version of `libtocksync_ymodem_read`: starts a read and returns
// immediately: `cb` is invoked (from a future `yield()`) when it completes.
static returncode_t libtock_ymodem_read(uint8_t* buffer, uint32_t length,
                                         libtock_ymodem_callback_read cb) {
  returncode_t err;

  subscribe_return_t sval = subscribe(DRIVER_NUM_YMODEM, UPCALL_READ_DONE,
                                       libtock_ymodem_read_done_trampoline, (void*) cb);
  err = tock_subscribe_return_to_returncode(sval);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_ymodem_set_readwrite_allow(buffer, length);
  if (err != RETURNCODE_SUCCESS) return err;

  return libtock_ymodem_command_read((int) length);
}

// Whether a receive is currently outstanding at the kernel (buffer
// allowed, subscribed, command issued) from a previous call to
// `libtocksync_ymodem_read_with_timeout` that timed out before completing.
// While true, the next call must not re-allow the buffer or re-issue the
// read command -- both are still active from before -- just wait on the
// same operation again.
static bool read_pending = false;

// Set by `read_done` when the pending read (if any) completes. Plain
// (non-volatile) `bool`/state to match `libtocksync_alarm_yield_for_with_timeout`,
// which this is built on: in this single-threaded, callbacks-only-run-from-yield
// execution model, `yield()` is already an optimization barrier, so volatile
// isn't needed.
static bool read_timeout_cb_fired          = false;
static returncode_t read_timeout_cb_result = RETURNCODE_SUCCESS;
static uint32_t read_timeout_cb_len        = 0;

static void read_timeout_read_done(returncode_t status, uint32_t length) {
  read_timeout_cb_result = status;
  read_timeout_cb_len    = length;
  read_timeout_cb_fired  = true;
}

returncode_t libtocksync_ymodem_read_with_timeout(uint8_t* buffer, uint32_t length, uint32_t* read,
                                                   uint32_t timeout_ms) {
  returncode_t err;

  if (!read_pending) {
    read_timeout_cb_fired  = false;
    read_timeout_cb_result = RETURNCODE_SUCCESS;
    read_timeout_cb_len    = 0;

    err = libtock_ymodem_read(buffer, length, read_timeout_read_done);
    if (err != RETURNCODE_SUCCESS) return err;

    read_pending = true;
  }

  // Race the read against the timeout; `read_timeout_cb_fired` is set by
  // `read_timeout_read_done` (an upcall, delivered from `yield()` like any
  // other) whenever the read actually completes, whether that happens
  // during this call or a later one.
  returncode_t wait_err = libtocksync_alarm_yield_for_with_timeout(&read_timeout_cb_fired, timeout_ms);
  if (wait_err != RETURNCODE_SUCCESS) {
    // Timed out. The receive is still outstanding at the kernel; leave it
    // that way and let the next call pick up its result.
    return RETURNCODE_ECANCEL;
  }

  read_pending = false;

  if (read_timeout_cb_result != RETURNCODE_SUCCESS) {
    return read_timeout_cb_result;
  }

  *read = read_timeout_cb_len;
  return RETURNCODE_SUCCESS;
}

