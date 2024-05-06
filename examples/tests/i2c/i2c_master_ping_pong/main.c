#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/interface/button.h>
#include <libtock/peripherals/i2c_master.h>

#define BUF_SIZE 16
#define FOLLOW_ADDRESS 0x41

uint8_t master_write_buf[BUF_SIZE];

// This is the callback for the button press.
// A button press indicates that this device should start the ping-pong
// exchange. First, change the address to the BUTTON_ADDRESS to avoid
// conflict with the other node, then send a message.
static void button_cb(
  __attribute__((unused)) returncode_t ret,
  __attribute__((unused)) int          btn_num,
  __attribute__ ((unused)) bool        val) {
  // Only the first press is meaningful
  static bool pressed = false;

  if (!pressed) {
    pressed = true;
    printf("Sending as master\n");

    TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_write_sync(FOLLOW_ADDRESS, master_write_buf, BUF_SIZE));
    printf("Sent.\n");
  } else {
    pressed = false;
  }
}

// This function sets up the I2C peripheral with needed buffers and prepares
// callbacks for I2C and button presses. Normal operation of this test takes
// place in the subsequent callbacks.
int main(void) {
  printf("I2C Master/Slave Ping-Pong\n");

  // Prepare buffers
  strcpy((char*) master_write_buf, "Hello friend.\n");

  // Set up I2C peripheral
  // Set up button peripheral to grab any button press

  int nbuttons;
  libtock_button_count(&nbuttons);
  if (nbuttons < 1) {
    printf("ERROR: This app requires that a board have at least one button.\n");
    exit(-1);
  }

  int j;
  for (j = 0; j < nbuttons; j++) {
    TOCK_EXPECT(RETURNCODE_SUCCESS, libtock_button_notify_on_press(j, button_cb));
  }

  while (1) {
    yield();
  }
}
