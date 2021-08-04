#include <stdbool.h>
#include <stdlib.h>

#include <button.h>
#include <i2c_master_slave.h>
#include <timer.h>

#define BUF_SIZE 16
#define LEADER_ADDRESS 0x40
#define FOLLOW_ADDRESS 0x41

uint8_t master_write_buf[BUF_SIZE];
uint8_t master_read_buf[BUF_SIZE];

bool is_leader = false;


// In response to a
static void i2c_callback(int callback_type,
                         __attribute__ ((unused)) int length,
                         __attribute__ ((unused)) int arg2,
                         __attribute__ ((unused)) void* userdata) {
  // Watching for GPIO interrupts holds us in a higher power state, so stop
  // doing that once we don't care about button presses any more (the first
  // time having sent or received a message)
  static bool any_message = false;
  if (!any_message) {
    int nbuttons;
    button_count(&nbuttons);
    int j;
    for (j = 0; j < nbuttons; j++) {
      button_disable_interrupt(j);
    }
  }

  if (callback_type == TOCK_I2C_CB_MASTER_READ) {
    printf("Master Reader\n");
    delay_ms(2500);

    printf("Read Buffer After: >%.*s<\n", BUF_SIZE, master_read_buf);
    memcpy((char*) master_write_buf, (char*) master_read_buf, BUF_SIZE);
    //master_write_buf[BUF_SIZE - 1] = '\0';
    printf("Write Buf Now: >%.*s<\n", BUF_SIZE, master_write_buf);
    TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_write(FOLLOW_ADDRESS, BUF_SIZE));
  } else if (callback_type == TOCK_I2C_CB_MASTER_WRITE) {
    delay_ms(1500);

    printf("Sending; >%.*s<\n", BUF_SIZE, master_write_buf);
    TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_write(FOLLOW_ADDRESS, BUF_SIZE));
  } else {
    printf("ERROR: Unexepected callback: type %d\n", callback_type);
  }
}

// This is the callback for the button press.
// A button press indicates that this device should start the ping-pong
// exchange. First, change the address to the BUTTON_ADDRESS to avoid
// conflict with the other node, then send a message.
static void button_cb(__attribute__((unused)) int btn_num,
                      __attribute__ ((unused)) int arg1,
                      __attribute__ ((unused)) int arg2,
                      __attribute__ ((unused)) void* userdata) {
  // Only the first press is meaningfull
  static bool pressed = false;

  if (!pressed) {
    pressed   = true;
    is_leader = true;

    printf("Getting Message\n");

    TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_read(FOLLOW_ADDRESS, BUF_SIZE));
  }
}

// This function sets up the I2C peripheral with needed buffers and prepares
// callbacks for I2C and button presses. Normal operation of this test takes
// place in the subsequent callbacks.
int main(void) {
  printf("I2C Master Read Test\n");

  // Prepare buffers
  strcpy((char*) master_write_buf, "Hello friend.\n");
  strcpy((char*) master_read_buf, "Hello am Master\n");

  // Set up I2C peripheral
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_callback(i2c_callback, NULL));
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_master_write_buffer(master_write_buf, BUF_SIZE));
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_master_read_buffer(master_read_buf, BUF_SIZE));
  
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_slave_address(LEADER_ADDRESS));
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_listen());

  // Set up button peripheral to grab any button press
  TOCK_EXPECT(RETURNCODE_SUCCESS, button_subscribe(button_cb, NULL));
  printf("Read Buffer Before: >%.*s<\n", BUF_SIZE, master_read_buf);


  int nbuttons;
  button_count(&nbuttons);
  if (nbuttons < 1) {
    printf("ERROR: This app requires that a board have at least one button.\n");
    exit(-1);
  }

  int j;
  for (j = 0; j < nbuttons; j++) {
    TOCK_EXPECT(RETURNCODE_SUCCESS, button_enable_interrupt(j));
  }
}