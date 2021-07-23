#include <stdbool.h>
#include <stdlib.h>

#include <button.h>
#include <i2c_master_slave.h>
#include <timer.h>

#define BUF_SIZE 16
#define LEADER_ADDRESS 0x40
#define FOLLOW_ADDRESS 0x41

uint8_t slave_write_buf[BUF_SIZE];
uint8_t slave_read_buf[BUF_SIZE];

int main(void) {
  printf("I2C Slave Read \n");

  // Prepare buffers

  // Set up I2C peripheral
  //TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_callback(i2c_callback, NULL));
  //TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_master_write_buffer(master_write_buf, BUF_SIZE));
  //TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_master_read_buffer(master_read_buf, BUF_SIZE));
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_enable_slave_read(BUF_SIZE));
  
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_slave_write_buffer(slave_write_buf, BUF_SIZE));
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_slave_read_buffer(slave_read_buf, BUF_SIZE));

  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_slave_address(FOLLOW_ADDRESS));
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_listen());
}
