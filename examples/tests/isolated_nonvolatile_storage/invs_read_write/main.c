#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <libtock-sync/storage/isolated_nonvolatile_storage.h>

static int test_all(void);
static int test(uint8_t* readbuf, uint8_t* writebuf, size_t size, size_t offset, size_t len);

int main(void) {
  printf("[TEST] Isolated Nonvolatile Storage\n");

  int r = test_all();
  if (r == 0) {
    printf("All tests succeeded\n");
  } else {
    printf("Failed with code %d\n", r);
  }

  return r;
}

static int test_all(void) {
  int num_bytes;
  libtocksync_isolated_nonvolatile_storage_get_number_bytes((uint32_t*) &num_bytes);
  printf("Have %i bytes of nonvolatile storage\n", num_bytes);

  int r;
  uint8_t readbuf[512];
  uint8_t writebuf[512];

  if ((r = test(readbuf, writebuf, 256, 0,  14)) != 0) return r;
  if ((r = test(readbuf, writebuf, 256, 20, 14)) != 0) return r;
  if ((r = test(readbuf, writebuf, 512, 0, 512)) != 0) return r;

  printf("Write to end of region (offset %d)\n", num_bytes - 512);
  if ((r = test(readbuf, writebuf, 512, num_bytes - 512, 500)) != 0) return r;

  printf("Write beyond end region, should fail (offset %d)\n", num_bytes);
  if ((r = test(readbuf, writebuf, 512, num_bytes, 501)) == 0) return -1;

  printf("Write starts beyond end region, should fail (offset %d)\n", num_bytes + 1);
  if ((r = test(readbuf, writebuf, 512, num_bytes + 1, 1)) == 0) return -1;

  printf("Write starts before start region, should fail (offset %d)\n", -1);
  if ((r = test(readbuf, writebuf, 512, -1, 1)) == 0) return -1;

  return 0;
}

static int test(uint8_t* readbuf, uint8_t* writebuf, size_t size, size_t offset, size_t len) {
  int ret;
  int length_written, length_read;

  printf("\tTest with size %d ...\n", size);

  for (size_t i = 0; i < len; i++) {
    writebuf[i] = i;
  }

  ret = libtocksync_isolated_nonvolatile_storage_write(offset, len, writebuf, size, &length_written);
  if (ret != RETURNCODE_SUCCESS) {
    printf("\tERROR calling write. returncode: %d\n", ret);
    return ret;
  }

  ret = libtocksync_isolated_nonvolatile_storage_read(offset, len, readbuf, size, &length_read);
  if (ret != RETURNCODE_SUCCESS) {
    printf("\tERROR calling read. returncode: %d\n", ret);
    return ret;
  }

  for (size_t i = 0; i < len; i++) {
    if (readbuf[i] != writebuf[i]) {
      printf("\tInconsistency between data written and read at index %u\n", i);
      return -1;
    }
  }

  return 0;
}
