#include <stdbool.h>
#include <stdlib.h>

#include <button.h>
#include <i2c_master_slave.h>
#include <timer.h>

#define BUF_SIZE 17
#define FOLLOW_ADDRESS 0x41

uint8_t slave_write_buf[BUF_SIZE];
uint8_t slave_read_buf[BUF_SIZE];

// In response to a
static void i2c_callback(int callback_type,
                         __attribute__ ((unused)) int length,
                         __attribute__ ((unused)) int arg2,
                         __attribute__ ((unused)) void* userdata) {
  
  if (callback_type == TOCK_I2C_CB_SLAVE_WRITE) {
    printf("CB: Slave write\n");
    printf("Write Buffer v2 is >%.*s<\n", BUF_SIZE, slave_write_buf);
  } else {
    printf("ERROR: Unexepected callback: type %d\n", callback_type);
  }
}

int main(void) {
  printf("I2C Slave Read \n");

  //Preparing buffer
  strncpy((char*) slave_write_buf, "0123456789ABCDEF", BUF_SIZE);
  printf("Buffer is >%.*s<\n", BUF_SIZE, slave_write_buf);

  // Set up I2C peripheral
  
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_callback(i2c_callback, NULL));
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_enable_slave_read(BUF_SIZE));
  
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_slave_write_buffer(slave_write_buf, BUF_SIZE));
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_slave_read_buffer(slave_read_buf, BUF_SIZE));

  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_slave_address(FOLLOW_ADDRESS));
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_listen());

}