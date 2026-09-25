#include <stdint.h>
#include <stdio.h>

#include <libtock-sync/peripherals/rng.h>
#include <libtock-sync/services/alarm.h>

// This test requests random bytes of varying lengths, with varying amounts
// of time between requests, to exercise the RNG driver under a wider range
// of usage patterns than a single fixed-size, fixed-interval request.

#define MAX_LEN 256
static uint8_t randbuf[MAX_LEN];

// Lengths to request, including small, odd, and boundary sizes.
static const uint32_t lengths[] = {
  1, 2, 3, 4, 7, 8, 15, 16, 31, 32, 63, 64, 100, 128, 200, 255, 256,
};
#define NUM_LENGTHS (sizeof(lengths) / sizeof(lengths[0]))

// Amount of time to wait, in milliseconds, before the next request.
static const uint32_t delays_ms[] = {
  0, 1, 10, 50, 100, 500, 1000,
};
#define NUM_DELAYS (sizeof(delays_ms) / sizeof(delays_ms[0]))

static void print_hex(const uint8_t* buf, uint32_t len) {
  for (uint32_t i = 0; i < len; i++) {
    printf("%02x", buf[i]);
  }
  printf("\n");
}

int main(void) {
  printf("[TEST] RNG (varying lengths and request spacing)\n");

  while (1) {
    for (uint32_t d = 0; d < NUM_DELAYS; d++) {
      uint32_t delay = delays_ms[d];

      for (uint32_t l = 0; l < NUM_LENGTHS; l++) {
        uint32_t len = lengths[l];
        int count    = 0;

        returncode_t ret = libtocksync_rng_get_random_bytes(randbuf, MAX_LEN, len, &count);

        printf("delay=%4lums len=%3lu -> ", (unsigned long) delay, (unsigned long) len);

        if (ret != RETURNCODE_SUCCESS) {
          printf("FAIL: %s\n", tock_strrcode(ret));
        } else if ((uint32_t) count != len) {
          printf("FAIL: requested %lu bytes, got %d\n", (unsigned long) len, count);
        } else {
          printf("OK: ");
          print_hex(randbuf, (uint32_t) count);
        }

        if (delay > 0) {
          libtocksync_alarm_delay_ms(delay);
        }
      }
    }
  }
}
