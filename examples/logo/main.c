/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <string.h>

#include <libtock/display/screen.h>
#include <libtock-sync/services/alarm.h>
#include <libtock-sync/display/screen.h>

#include "logo.h"

unsigned char logo_b[sizeof(logo_bin)];

int main(void) {
  uint32_t width, height;
  libtock_screen_get_resolution(&width, &height);

  printf("Using screen with resolution %ld %ld\n", width, height);
  memcpy(logo_b, logo_bin, sizeof(logo_b));

  libtocksync_screen_set_frame(0, 0, width, height);
  while (1) {

    libtocksync_screen_write(logo_b, logo_bin_len, logo_bin_len);

    libtocksync_alarm_delay_ms(50);

    unsigned char prev_line[22];
    memcpy(prev_line, logo_b, 22);
    for (unsigned int i = 22; i < logo_bin_len; i += 22) {
      unsigned char line[22];
      memcpy(line, logo_b + i, 22);
      memcpy(logo_b + i, prev_line, 22);
      memcpy(prev_line, line, 22);
    }
    memcpy(logo_b, prev_line, 22);

  }
  return 0;
}
