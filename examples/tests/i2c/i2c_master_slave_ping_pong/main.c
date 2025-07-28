#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/interface/button.h>
#include <libtock/interface/syscalls/button_syscalls.h>
#include <libtock/peripherals/i2c_master_slave.h>

#define BUF_SIZE 16
#define LEADER_ADDRESS 0x40
#define FOLLOW_ADDRESS 0x41

uint8_t master_write_buf[BUF_SIZE];
uint8_t master_read_buf[BUF_SIZE];
uint8_t slave_write_buf[BUF_SIZE];
uint8_t slave_read_buf[BUF_SIZE];
bool is_leader = false;


// In response to a
static void i2c_callback(int                            callback_type,
                         __attribute__ ((unused)) int   length,
                         __attribute__ ((unused)) int   arg2,
                         __attribute__ ((unused)) void* userdata) {
  // Watching for GPIO interrupts holds us in a higher power state, so stop
  // doing that once we don't care about button presses any more (the first
  // time having sent or received a message)
  static bool any_message = false;
  if (!any_message) {
    int nbuttons;
    libtock_button_count(&nbuttons);
    int j;
    for (j = 0; j < nbuttons; j++) {
      libtock_button_command_disable_interrupt(j);
    }
  }

  if (callback_type == TOCK_I2C_CB_MASTER_WRITE) {
    printf("CB: Master write\n");
    TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_listen());
  } else if (callback_type == TOCK_I2C_CB_SLAVE_WRITE) {
    printf("CB: Slave write\n");
    libtocksync_alarm_delay_ms(2500);

    printf("%s sending\n", is_leader ? "Leader" : "Follower");
    TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_write(is_leader ? FOLLOW_ADDRESS : LEADER_ADDRESS, BUF_SIZE));
  } else {
    printf("ERROR: Unexepected callback: type %d\n", callback_type);
  }
}

// This is the callback for the button press.
// A button press indicates that this device should start the ping-pong
// exchange. First, change the address to the BUTTON_ADDRESS to avoid
// conflict with the other node, then send a message.
static void button_cb(
  __attribute__((unused)) returncode_t ret,
  __attribute__((unused)) int          btn_num,
  __attribute__ ((unused)) bool        val) {
  // Only the first press is meaningfull
  static bool pressed = false;

  if (!pressed) {
    pressed   = true;
    is_leader = true;

    printf("Switching to master\n");

    TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_slave_address(LEADER_ADDRESS));
    TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_write(FOLLOW_ADDRESS, BUF_SIZE));
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
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_callback(i2c_callback, NULL));
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_master_write_buffer(master_write_buf, BUF_SIZE));
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_master_read_buffer(master_read_buf, BUF_SIZE));
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_slave_write_buffer(slave_write_buf, BUF_SIZE));
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_slave_read_buffer(slave_read_buf, BUF_SIZE));

  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_slave_address(FOLLOW_ADDRESS));
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_listen());

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
