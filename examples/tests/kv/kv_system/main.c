#include <stdio.h>
#include <stdlib.h>

#include <libtock/interface/console.h>
#include <libtock/storage/kv.h>

#define KEY_LEN  16
#define DATA_LEN 32

uint8_t key_buf[KEY_LEN]   = "First Key";
uint8_t data_buf[DATA_LEN] = "My secret key that no one knows";
uint8_t out_buf[DATA_LEN]  = "Just junk";

static void kv_cb(int                          result,
                  __attribute__ ((unused)) int length,
                  __attribute__ ((unused)) int verified,
                  void*                        con) {
  if (result != 0) {
    printf("Failure %d\n", result);
    exit(-1);
  }

  *(bool*)con = true;
}

int main(void) {
  int i;
  bool con = false;

  printf("[TEST] KV System Example Test\r\n");

  TOCK_EXPECT(true, libtock_kv_exists());

  printf("Loading in the key...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, libtock_kv_set_readonly_allow_key_buffer(key_buf, KEY_LEN));
  printf("   done\r\n");

  printf("Loading in the input buf...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, libtock_kv_set_readonly_allow_input_buffer(data_buf, DATA_LEN));
  printf("   done\r\n");

  printf("Setting up the callback...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, libtock_kv_set_upcall(kv_cb, &con));
  printf("   done\r\n");

  printf("Adding the key...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, libtock_kv_command_set());
  printf("   done\r\n");

  yield_for(&con);
  con = false;

  printf("Finish adding the key\r\n");

  printf("Loading in the output buf...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, libtock_kv_set_readwrite_allow_output_buffer(out_buf, DATA_LEN));
  printf("   done\r\n");

  printf("Getting the key...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, libtock_kv_command_get());
  printf("   done\r\n");

  yield_for(&con);
  con = false;

  printf("Finish retrieving the key: \"%s\"\r\n", out_buf);

  for (i = 0; i < DATA_LEN; i++) {
    if (out_buf[i] != data_buf[i]) {
      printf("Comparison failure at %d\n", i);
      printf("Expected value: 0x%x\n", data_buf[i]);
      printf(" But got value: 0x%x\n", out_buf[i]);
      exit(-1);
    }
  }

  printf("Deleting the key...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, libtock_kv_command_delete());
  yield_for(&con);
  con = false;
  printf("   done\r\n");

  printf("Getting the key, again...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, libtock_kv_command_get());
  printf("This should fail\r\n");

  yield_for(&con);
  con = false;

  return 0;
}
