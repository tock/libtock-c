#include <stdlib.h>
#include <string.h>

#include <libtock-sync/display/text_screen.h>
#include <timer.h>
#include <tock.h>

int main(void) {
  returncode_t ret;

  uint8_t* buffer = calloc(15, 1);

  ret = libtocksync_text_screen_display_on();
  if (ret == RETURNCODE_SUCCESS) {
    ret = libtocksync_text_screen_set_cursor(0, 0);
    memcpy(buffer, "Hello!", 6);
    ret = libtocksync_text_screen_write(buffer, 15, 6);

    delay_ms(5000);

    ret = libtocksync_text_screen_set_cursor(0, 1);
    memcpy(buffer, "Goodbyee!", 9);
    ret = libtocksync_text_screen_write(buffer, 15, 9);

    delay_ms(2000);
    ret = libtocksync_text_screen_clear();
  }
  return 0;
}
