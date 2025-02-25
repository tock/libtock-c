#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <libtock-sync/storage/isolated_nonvolatile_storage.h>

static int test_all(void);
static int test(uint8_t* readbuf, size_t readsize, uint8_t* writebuf, size_t writesize, size_t offset);

static size_t min(size_t a, size_t b) {
  if (a < b) return a;
  return b;
}

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
  uint64_t num_bytes_total;
  libtocksync_isolated_nonvolatile_storage_get_number_bytes(&num_bytes_total);
  int num_bytes = num_bytes_total;
  printf("Have %i bytes of nonvolatile storage\n", num_bytes);

  int r;
  uint8_t readbuf[512];
  uint8_t writebuf[512];

  if ((r = test(readbuf, 14, writebuf, 256, 0)) != 0) return r;
  if ((r = test(readbuf, 14, writebuf, 256, 20)) != 0) return r;
  if ((r = test(readbuf, 512, writebuf, 512, 0)) != 0) return r;

  printf("Write to end of region (offset %d)\n", num_bytes - 512);
  if ((r = test(readbuf, 512, writebuf, 500, num_bytes - 512)) != 0) return r;

  printf("Write beyond end region, should fail (offset %d)\n", num_bytes);
  if ((r = test(readbuf, 512, writebuf, 501, num_bytes)) == 0) return -1;

  printf("Write starts beyond end region, should fail (offset %d)\n", num_bytes + 1);
  if ((r = test(readbuf, 512, writebuf, 1, num_bytes + 1)) == 0) return -1;

  printf("Write starts before start region, should fail (offset %d)\n", -1);
  if ((r = test(readbuf, 512, writebuf, 512, -1)) == 0) return -1;

  return 0;
}

static int test(uint8_t* readbuf, size_t readsize, uint8_t* writebuf, size_t writesize, size_t offset) {
  int ret;

  printf("\tTest with read size %d and write size %d ...\n", readsize, writesize);

  for (size_t i = 0; i < writesize; i++) {
    writebuf[i] = i;
  }

  ret = libtocksync_isolated_nonvolatile_storage_write(offset, writebuf, writesize);
  if (ret != RETURNCODE_SUCCESS) {
    printf("\tERROR calling write. returncode: %d\n", ret);
    return ret;
  }

  ret = libtocksync_isolated_nonvolatile_storage_read(offset, readbuf, readsize);
  if (ret != RETURNCODE_SUCCESS) {
    printf("\tERROR calling read. returncode: %d\n", ret);
    return ret;
  }

  for (size_t i = 0; i < min(readsize, writesize); i++) {
    if (readbuf[i] != writebuf[i]) {
      printf("\tInconsistency between data written and read at index %u\n", i);
      return -1;
    }
  }

  return 0;
}
