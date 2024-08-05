#include "distance.h"

// Internal upcall for passing to the syscall driver.
static void distance_upcall(int                          dist,
                               __attribute__ ((unused)) int unused,
                               __attribute__ ((unused)) int unused1,
                               void*                        opaque) {
  libtock_distance_callback cb = (libtock_distance_callback) opaque;
  cb(RETURNCODE_SUCCESS, dist);
}

returncode_t libtock_distance_read(libtock_distance_callback cb) {
  returncode_t ret;

  ret = libtock_distance_set_upcall(distance_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_distance_command_read();
  return ret;
}
