#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <libtock-sync/storage/isolated_nonvolatile_storage.h>


static bool test_exists(void) {
  bool exists = libtock_isolated_nonvolatile_storage_exists();

  if (exists) {
    return true;
  }
  return false;
}

static bool test_size(void) {
  uint64_t num_bytes;
  returncode_t ret;

  ret = libtocksync_isolated_nonvolatile_storage_get_number_bytes(&num_bytes);

  if (ret == RETURNCODE_ENOSUPPORT) {
    return true;
  }
  return false;
}

static bool test_read(void) {
  returncode_t ret;

  uint8_t readbuf[512];

  uint32_t offset = 0;
  uint32_t length = 10;

  ret = libtocksync_isolated_nonvolatile_storage_read(offset, readbuf, length);
  if (ret == RETURNCODE_ENOSUPPORT) {
    return true;
  }
  return false;
}


static bool test_write(void) {
  returncode_t ret;

  uint8_t writebuf[512] = {0x3f};

  uint32_t offset = 0;
  uint32_t length = 10;

  ret = libtocksync_isolated_nonvolatile_storage_write(offset, writebuf, length);
  if (ret == RETURNCODE_ENOSUPPORT) {
    return true;
  }
  return false;
}


int main(void) {
  printf("[TEST] Isolated Nonvolatile Storage - No Permissions\n");

  printf("[INVS TEST] Exists: ");
  if (test_exists()) {
    printf("Success\n");
  } else {
    printf("Fail\n");
  }

  printf("[INVS TEST] Size: ");
  if (test_size()) {
    printf("Success\n");
  } else {
    printf("Fail\n");
  }

  printf("[INVS TEST] Read: ");
  if (test_read()) {
    printf("Success\n");
  } else {
    printf("Fail\n");
  }

  printf("[INVS TEST] Write: ");
  if (test_write()) {
    printf("Success\n");
  } else {
    printf("Fail\n");
  }

  return 0;
}
