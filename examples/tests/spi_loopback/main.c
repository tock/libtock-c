#include <stdio.h>
#include <string.h>

#include <libtock/peripherals/spi_controller.h>
#include <libtock/tock.h>

#define BUF_SIZE 32

// Arbitrary, conservative choice. As of 2026-08-26 this has only been
// tested on the QEMU ARM MPS2 AN385/AN386 boards; untested on other
// boards/hardware, so lower it if a real target can't keep up.
#define SPI_RATE_HZ 400000

static uint8_t write_buf[BUF_SIZE];
static uint8_t read_buf[BUF_SIZE];
static bool done = false;

static void spi_cb(returncode_t ret __attribute__((unused))) {
  done = true;
}

static void print_buf(const char* label, const uint8_t* buf, size_t len) {
  printf("%s:", label);
  for (size_t i = 0; i < len; i++) {
    printf(" %02x", buf[i]);
  }
  printf("\n");
}

int main(void) {
  if (!libtock_spi_controller_exists()) {
    printf("SPI FAIL: driver not present\n");
    return 1;
  }

  // `read_buf` is pre-filled with a pattern disjoint from `write_buf`'s
  // (offset by BUF_SIZE) so a no-op read_write (e.g. an unimplemented
  // syscall that leaves the buffer untouched) can't accidentally match
  // and produce a false PASS.
  for (unsigned i = 0; i < BUF_SIZE; i++) {
    write_buf[i] = (uint8_t) i;
    read_buf[i]  = (uint8_t) (i + BUF_SIZE);
  }

  returncode_t err;

  // As of 2026-08-26, `set_chip_select()` is unimplemented on every Tock
  // board -- the spi_controller capsule always returns NOSUPPORT here
  // until multiple chip selects are supported. Check for exactly that so
  // this test fails loudly if that ever changes, rather than silently
  // masking a real error.
  err = libtock_spi_controller_set_chip_select(0);
  if (err != RETURNCODE_ENOSUPPORT) {
    printf("SPI FAIL: set_chip_select returned %d, expected ENOSUPPORT\n", err);
    return 1;
  }

  err = libtock_spi_controller_set_rate(SPI_RATE_HZ);
  if (err != RETURNCODE_SUCCESS) {
    printf("SPI FAIL: set_rate returned %d\n", err);
    return 1;
  }

  err = libtock_spi_controller_read_write(write_buf, read_buf, BUF_SIZE, spi_cb);
  if (err != RETURNCODE_SUCCESS) {
    printf("SPI FAIL: read_write returned %d\n", err);
    return 1;
  }

  printf("SPI transaction initialized. Waiting for callback.\n");
  yield_for(&done);

  if (memcmp(write_buf, read_buf, BUF_SIZE) == 0) {
    printf("SPI PASS\n");
  } else {
    printf("SPI FAIL: loopback mismatch\n");
    print_buf("write", write_buf, BUF_SIZE);
    print_buf("read ", read_buf, BUF_SIZE);
  }

  return 0;
}
