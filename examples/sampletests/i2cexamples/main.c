#include <stdbool.h>
#include <stdlib.h>

#include <button.h>
#include <i2c_master_slave.h>
#include <timer.h>

#define BUF_SIZE 6
#define LEADER_ADDRESS 0x40
#define FOLLOW_ADDRESS 0x41

uint8_t slave_write_buf[BUF_SIZE];
uint8_t slave_read_buf[BUF_SIZE];

// In response to a
static void i2c_callback(int callback_type,
                         __attribute__ ((unused)) int length,
                         __attribute__ ((unused)) int arg2,
                         __attribute__ ((unused)) void* userdata) {
  
  printf("callback Num: %d\n", callback_type);
  if (callback_type == TOCK_I2C_CB_MASTER_WRITE) {
    printf("CB: Master write\n");
  } else if (callback_type == TOCK_I2C_CB_SLAVE_WRITE) {
    printf("CB: Slave write\n");
    //printf("Read Buffer v2 is >%.*s<\n", BUF_SIZE, slave_read_buf);
    //printf("Write Buffer v2 is >%.*s<\n", BUF_SIZE, slave_write_buf);

    delay_ms(2500);

    printf("%s sending\n", FOLLOW_ADDRESS);
    //TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_write(LEADER_ADDRESS, BUF_SIZE));
    printf("Write Buffer v2 is >%.*s<\n", BUF_SIZE, slave_write_buf);
  } else {
    printf("ERROR: Unexepected callback: type %d\n", callback_type);
  }
}

int main(void) {
  printf("I2C Slave Read \n");


  strncpy(slave_write_buf, "012345", BUF_SIZE);
  int buffer_to_write = {0,1,2,3,4,5};
  printf("Buffer is >%.*s<\n", BUF_SIZE, slave_write_buf);
  // Prepare buffers

  // Set up I2C peripheral
  //TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_callback(i2c_callback, NULL));
  //TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_master_write_buffer(master_write_buf, BUF_SIZE));
  //TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_master_read_buffer(master_read_buf, BUF_SIZE));
  
  //TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_callback(i2c_callback, NULL));
  //TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_enable_slave_read(BUF_SIZE));
  
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_slave_write_buffer(slave_write_buf, BUF_SIZE));
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_slave_read_buffer(slave_read_buf, BUF_SIZE));

  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_set_slave_address(FOLLOW_ADDRESS));
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_listen());
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_write_sync(LEADER_ADDRESS, BUF_SIZE, buffer_to_write));
  //TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_slave_enable_slave_read(BUF_SIZE));

  printf("Read Buffer is >%.*s<\n", BUF_SIZE, slave_read_buf);
  printf("Write Buffer is >%.*s<\n", BUF_SIZE, slave_write_buf);
}
