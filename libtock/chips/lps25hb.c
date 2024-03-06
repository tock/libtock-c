#include "tock.h"

#include "lps25hb.h"
#include "syscalls/lps25hb_syscalls.h"

// Internal callback for faking synchronous reads
static void lps25hb_upcall(int                          value,
                           __attribute__ ((unused)) int unused1,
                           __attribute__ ((unused)) int unused2,
                           void*                        opaque) {
  libtock_lps25hb_callback cb = (libtock_lps25hb_callback) opaque;
  cb(RETURNCODE_SUCCESS, value);
}

returncode_t libtock_lps25hb_get_pressure(libtock_lps25hb_callback cb) {
  returncode_t err;

  err = libtock_lps25hb_set_upcall(lps25hb_upcall, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_lps25hb_command_get_pressure();
  return err;
}
