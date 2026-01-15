#include <stdio.h>
#include <string.h>
#include <libtock-sync/services/alarm.h>

#include <libtock-sync/peripherals/sdi12.h>

#define CMD_LEN 4
#define RESPONSE_LEN 11
#define MEASUREMENT_LEN 60

uint8_t cmd_buf[CMD_LEN]; 
uint8_t response_buf[RESPONSE_LEN];
uint8_t measurement_buf[MEASUREMENT_LEN];

int main(void) {
  returncode_t ret;
  printf("[TEST] SDI12\r\n");

  if (!libtocksync_sdi12_exists()) {
    printf("[FAIL] No SDI12 driver.\n");
    return -2;
  } else {
    printf("[PASS] SDI12 driver exists.\n");
  }

  // Step 1: Send measurement command 0M!
  printf("Sending 0M! command...\n");
  cmd_buf[0] = '0';
  cmd_buf[1] = 'M';
  cmd_buf[2] = '!';
  
  ret = libtocksync_sdi12_write_and_receive(cmd_buf, 3, response_buf, RESPONSE_LEN);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[FAIL] 0M! failed with code %d.\n", ret);
    return -1;
  }
  
  printf("0M! response: ");
  for (int i = 4; i < RESPONSE_LEN; i++) {
    char c = response_buf[i];
    if (c == '\0') continue;
    if (c >= 32 && c <= 126) printf("%c", c);
  }
  printf("\n");

  // Step 2: Parse time and wait for measurement
  int wait_seconds = 0;
  if (response_buf[5] >= '0' && response_buf[5] <= '9') {
      wait_seconds = (response_buf[5] - '0') * 100;
  }
  if (response_buf[6] >= '0' && response_buf[6] <= '9') {
      wait_seconds += (response_buf[6] - '0') * 10;
  }
  if (response_buf[7] >= '0' && response_buf[7] <= '9') {
      wait_seconds += (response_buf[7] - '0');
  }
  printf("Waiting %d seconds for measurement...\n", wait_seconds);
  libtocksync_alarm_delay_ms(wait_seconds * 1000); // delay in milliseconds

  // Step 3: Send data command 0D0!
  printf("Sending 0D0! command...\n");
  cmd_buf[0] = '0';
  cmd_buf[1] = 'D';
  cmd_buf[2] = '0';
  cmd_buf[3] = '!';
  
  ret = libtocksync_sdi12_write_and_receive(cmd_buf, 4, measurement_buf, MEASUREMENT_LEN);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[FAIL] 0D0! failed with code %d.\n", ret);
    return -1;
  }
  
  printf("Measurement data: ");
  for (int i = 5; i < MEASUREMENT_LEN; i++) { 
    char c = measurement_buf[i];
    if (c == '\0') continue;
    if (c == '\r' || c == '\n') break;
    if (c >= 32 && c <= 126) printf("%c", c);
  }
  printf("\n");

  printf("[SUCCESS] All SDI12 tests have passed.\n");
  return 0;
}