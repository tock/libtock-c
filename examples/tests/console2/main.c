// Licensed under the Apache License, Version 2.0 or the MIT License.
// SPDX-License-Identifier: Apache-2.0 OR MIT
// Copyright Tock Contributors 2026.



#include <stdio.h>

#include <libtock-sync/services/alarm.h>

#include "console2.h"



int main(void) {
  printf("[Console2] Writing to second serial console.\n");

  const char TX[]  = "The is the second serial console.";
  uint32_t written = 0;

  // Write the the second console in a continuous loop.
  int count = 0;
  while (1) {
    char out[256];
    int len = snprintf(out, 256, "%s (%d)\n", TX, count);
    console2_sync_write((uint8_t*) out, len, &written);
    count += 1;

    libtocksync_alarm_delay_ms(603);
  }

  return 0;
}
