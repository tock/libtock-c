#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <libtock-sync/storage/isolated_nonvolatile_storage.h>




struct nv_data {
  bool fired;
  returncode_t ret;
  int length;
  int storage_size;
};

static struct nv_data result = {.fired = false};

// static void get_number_bytes_cb(returncode_t ret, int number_bytes) {
//   result.fired        = true;
//   result.ret          = ret;
//   result.storage_size = number_bytes;
// }

// static void write_cb(returncode_t ret, int length) {
//   result.fired  = true;
//   result.ret    = ret;
//   result.length = length;
// }

static void read_cb(returncode_t ret, int length) {
  result.fired  = true;
  result.ret    = ret;
  result.length = length;
}

static void read_done(int                          ret,
                      int                          length,
                      __attribute__ ((unused)) int arg2,
                      void*                        opaque) {
  libtock_isolated_nonvolatile_storage_callback_read cb = (libtock_isolated_nonvolatile_storage_callback_read) opaque;
  cb(tock_status_to_returncode(ret), length);
}


static uint32_t get_region_size(void) {
   uint32_t num_bytes;
    libtocksync_isolated_nonvolatile_storage_get_number_bytes(&num_bytes);
    return  num_bytes;
}



static bool test_exists(void) {
  bool exists = libtock_isolated_nonvolatile_storage_exists();

  if (exists) {
    return true;
  }
  return false;
}

static bool test_size(void) {
  uint32_t num_bytes;
  returncode_t ret;

  ret = libtocksync_isolated_nonvolatile_storage_get_number_bytes(&num_bytes);

  if (ret == RETURNCODE_SUCCESS) {
    if (num_bytes > 0) {
      return true;
    }
  }
  return false;
}

static bool test_read(void) {
  returncode_t ret;

  uint8_t readbuf[512];

  uint32_t offset = 0;
  uint32_t length = 10;
  uint32_t length_read;

  ret = libtocksync_isolated_nonvolatile_storage_read(offset, length, readbuf, sizeof(readbuf), &length_read);
  if (ret == RETURNCODE_SUCCESS) {
    if (length_read == length) {
      return true;
    }
  }
  return false;
}

static bool test_read_zerobuffer(void) {
  returncode_t ret;

  uint8_t readbuf[512];

  uint32_t offset = 0;
  uint32_t length = 10;
  uint32_t length_read;

  ret = libtocksync_isolated_nonvolatile_storage_read(offset, length, readbuf, 0, &length_read);
  if (ret == RETURNCODE_ERESERVE) {
      return true;
  }
  return false;
}

static bool test_read_buffertoosmall(void) {
  returncode_t ret;

  uint8_t readbuf[512];

  uint32_t offset = 0;
  uint32_t length = 10;
  uint32_t length_read;

  ret = libtocksync_isolated_nonvolatile_storage_read(offset, length, readbuf, 5, &length_read);
  if (ret == RETURNCODE_SUCCESS) {
    if (length_read == 5) {
      return true;
    }
  }
  return false;
}

static bool test_read_fail_noallow(void) {
  returncode_t ret;

  uint32_t offset = 0;
  uint32_t length = 10;

  result.fired = false;

  ret = libtock_isolated_nonvolatile_storage_set_upcall_read_done(read_done, read_cb);
  if (ret != RETURNCODE_SUCCESS) return false;

  ret = libtock_isolated_nonvolatile_storage_set_allow_readwrite_read_buffer(NULL, 0);
  if (ret != RETURNCODE_SUCCESS) return false;

  ret = libtock_isolated_nonvolatile_storage_command_read(offset, length);
  if (ret != RETURNCODE_SUCCESS) return false;

  yield_for(&result.fired);

  if (result.ret == RETURNCODE_ERESERVE) {
    return true;
  }
  return false;
}

static bool test_read_fail_notwithinregion(void) {
  returncode_t ret;

  uint8_t readbuf[512];

  uint32_t offset;
  uint32_t length;
  uint32_t length_read;


  offset  = get_region_size() -5;
  length = 10;
  ret = libtocksync_isolated_nonvolatile_storage_read(offset, length, readbuf, sizeof(readbuf), &length_read);
  printf("returncode %d\n",ret);
  if (ret != RETURNCODE_EINVAL) return false;

  offset = get_region_size() + 10;
  length = 10;
  ret = libtocksync_isolated_nonvolatile_storage_read(offset, length, readbuf, sizeof(readbuf), &length_read);
  printf("returncode %d\n",ret);
  if (ret != RETURNCODE_EINVAL) return false;

  offset = 0;
  length = get_region_size() + 1;
  ret = libtocksync_isolated_nonvolatile_storage_read(offset, length, readbuf, sizeof(readbuf), &length_read);
  printf("returncode %d\n",ret);
  if (ret != RETURNCODE_EINVAL) return false;

  offset = get_region_size() - 1;
  length = 2;
  ret = libtocksync_isolated_nonvolatile_storage_read(offset, length, readbuf, sizeof(readbuf), &length_read);
  printf("returncode %d\n",ret);
  if (ret != RETURNCODE_EINVAL) return false;

  return true;
}



int main(void) {
  printf("[TEST] Isolated Nonvolatile Storage - Single\n");

  printf("[INVS TEST] Exists: ");
  if (test_exists()) printf("Success\n");
  else printf("Fail\n");

  printf("[INVS TEST] Size: ");
  if (test_size()) printf("Success\n");
  else printf("Fail\n");

  printf("[INVS TEST] Read: ");
  if (test_read()) printf("Success\n");
  else printf("Fail\n");

  printf("[INVS TEST] Read Zero Buffer: ");
  if (test_read_zerobuffer()) printf("Success\n");
  else printf("Fail\n");

  printf("[INVS TEST] Read Buffer Too Small: ");
  if (test_read_buffertoosmall()) printf("Success\n");
  else printf("Fail\n");

  printf("[INVS TEST] Read Fail No Allow: ");
  if (test_read_fail_noallow()) printf("Success\n");
  else printf("Fail\n");

  printf("[INVS TEST] Read Fail Not Within Region: ");
  if (test_read_fail_notwithinregion()) printf("Success\n");
  else printf("Fail\n");


  return 0;
}
