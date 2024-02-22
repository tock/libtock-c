#include <stdint.h>
#include <stdio.h>

#include <internal/nonvolatile_storage.h>
#include <nonvolatile_storage.h>







static void read_done(int                            length,
                      __attribute__ ((unused)) int   arg1,
                      __attribute__ ((unused)) int   arg2,
                       void* opaque) {
  nonvolatile_storage_callback cb = (nonvolatile_storage_callback) opaque;
   cb(RETURNCODE_SUCCESS, length);
}

static void write_done(int                            length,
                       __attribute__ ((unused)) int   arg1,
                       __attribute__ ((unused)) int   arg2,
                       void* opaque) {
  nonvolatile_storage_callback cb = (nonvolatile_storage_callback) opaque;
   cb(RETURNCODE_SUCCESS, length);
}


returncode_t nonvolatile_storage_read(uint8_t* buffer, uint32_t length, uint32_t address, nonvolatile_storage_callback cb) {
  returncode_t ret;

  ret = nonvolatile_storage_internal_read_buffer(buffer, length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = nonvolatile_storage_internal_read_done_subscribe(read_done, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = nonvolatile_storage_internal_read(address, length);
  return ret;
}

returncode_t nonvolatile_storage_write(uint8_t* buffer, uint32_t length, uint32_t address, nonvolatile_storage_callback cb) {
  returncode_t ret;

  ret = nonvolatile_storage_internal_write_buffer(buffer, length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = nonvolatile_storage_internal_write_done_subscribe(write_done, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = nonvolatile_storage_internal_write(address, length);
  return ret;
}



struct data {
  bool fired;
  int length;
  returncode_t result;
};

static struct data result = { .fired = false };


static void nv_storage_done(returncode_t ret, int length) {
  result.fired = true;
  result.length = length;
  result.result = ret;
}



returncode_t nonvolatile_storage_read_sync(uint8_t* buffer, uint32_t length, uint32_t address) {
  returncode_t ret;
  result.fired = false;


  ret = nonvolatile_storage_read(buffer, length, address, nv_storage_done);
  if (ret != RETURNCODE_SUCCESS) return ret;


  yield_for(&result.fired);
  return RETURNCODE_SUCCESS;
}

returncode_t nonvolatile_storage_write_sync(uint8_t* buffer, uint32_t length, uint32_t address) {
  returncode_t ret;
  result.fired = false;


  ret = nonvolatile_storage_write(buffer, length, address, nv_storage_done);
  if (ret != RETURNCODE_SUCCESS) return ret;


  yield_for(&result.fired);
  return RETURNCODE_SUCCESS;
}


