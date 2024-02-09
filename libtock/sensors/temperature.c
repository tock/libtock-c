#include "tock.h"

#include "temperature.h"
#include "temperature_syscalls.h"



// Internal upcall for passing to the syscall driver.
static void temperature_temp_upcall(int                          temp,
                                    __attribute__ ((unused)) int unused,
                                    __attribute__ ((unused)) int unused1,
                                    void*                        opaque) {
  temperature_callback cb = (temperature_callback) opaque;
  cb(RETURNCODE_SUCCESS, temp);
}



returncode_t temperature_read(temperature_callback cb) {
  returncode_t ret;

  ret = temperature_set_upcall(temperature_temp_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = temperature_command_read();
  return ret;
}
