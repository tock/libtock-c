#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/peripherals/crc.h>
#include <libtock-sync/peripherals/rng.h>
#include <libtock-sync/services/alarm.h>

struct test_case {
  libtock_crc_alg_t alg;
  uint32_t output;
  char *input;
};

#define CASE(alg, output, input) char input_ ## alg ## _ ## output [] = input;
#include "test_cases.h"
#undef CASE

static struct test_case test_cases[] = {
#define CASE(alg, output, input) \
  { alg, output, input_ ## alg ## _ ## output },
#include "test_cases.h"
#undef CASE
};

int n_test_cases = sizeof(test_cases) / sizeof(struct test_case);

uint32_t procid;

int main(void) {
  int r;

  printf("[CRC Test] This app tests the CRC syscall interface\n");

  // Get a random number to distinguish this app instance
  int num_bytes;
  r = libtocksync_rng_get_random_bytes((uint8_t *) &procid, 4, 4, &num_bytes);
  if (r != RETURNCODE_SUCCESS || num_bytes != 4) {
    printf("RNG failure\n");
    exit(1);
  }

  if (!libtock_crc_exists()) {
    printf("CRC driver does not exist\n");
    exit(1);
  }

  while (1) {
    for (int test_index = 0; test_index < n_test_cases; test_index++) {
      struct test_case *t = &test_cases[test_index];
      uint32_t result;
      if ((r = libtocksync_crc_compute((uint8_t*) t->input, strlen(t->input), t->alg, &result)) != RETURNCODE_SUCCESS) {
        printf("CRC compute failed: %s\n", tock_strrcode(r));
        exit(1);
      }

      printf("[%8lx] Case %2d: ", procid, test_index);
      if (r == RETURNCODE_SUCCESS) {
        printf("result=%08lx ", result);
        if (result == t->output) {
          printf("(OK)");
        } else {
          printf("(Expected %08lx)", t->output);
        }
      } else {
        printf("failed with status %d\n", r);
      }
      printf("\n");
    }

    printf("\n");
    libtocksync_alarm_delay_ms(1000);
  }
}
