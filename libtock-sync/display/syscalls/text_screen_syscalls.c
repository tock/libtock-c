#include "text_screen_syscalls.h"

returncode_t libtocksync_text_screen_yield_wait_for_done(void) {
  yield_waitfor_return_t ywf;
  ywf = yield_wait_for(DRIVER_NUM_TEXT_SCREEN, 0);
  return (returncode_t) ywf.data0;
}

returncode_t libtocksync_text_screen_yield_wait_for_size(uint32_t* width, uint32_t* height) {
  yield_waitfor_return_t ywf;
  returncode_t ret;
  ywf = yield_wait_for(DRIVER_NUM_TEXT_SCREEN, 0);
  ret = (returncode_t) ywf.data0;
  if (ret != RETURNCODE_SUCCESS) return ret;
  *width  = (uint32_t) ywf.data1;
  *height = (uint32_t) ywf.data2;
  return ret;
}
