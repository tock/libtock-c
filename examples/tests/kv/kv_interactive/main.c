#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock/interface/console.h>
#include <libtock-sync/storage/kv.h>

#define KEY_LEN  64
#define DATA_LEN 64

uint8_t key_buf[KEY_LEN];
uint8_t data_buf[DATA_LEN];
uint8_t value_buf[DATA_LEN];

char read_buf[DATA_LEN];

static int get_command(void) {
  int idx = 0;
  memset(read_buf, 0, 64);

  // Read in characters until a new line
  while (1) {
    int c = getch();

    if (c == RETURNCODE_FAIL) {
      printf("\ngetch() failed!\n");
      return c;

    } else {
      char in = c;

      if (in == '\n' || in == '\r') {
        return 0;

      } else {
        // If this is a valid number record it
        read_buf[idx] = in;
        idx += 1;

        // If our buffer is full, quit
        if (idx >= 64) {
          return 0;
        }
      }
    }
  }
}

static int find_end(int start) {
  for (int i = start; i < DATA_LEN; i++) {
    if (read_buf[i] == ' ' || read_buf[i] == '\n' || read_buf[i] == '\0') {
      return i;
    }
  }
  return DATA_LEN;
}

int main(void) {
  int ret;

  printf("[TEST] Text interface to KV store\n");
  printf("\n");
  printf("Please enter a command: get, set/add/update, or delete\n");
  printf("   get <key>\n");
  printf("   set <key> <value>\n");
  printf("   add <key> <value>\n");
  printf("   update <key> <value>\n");
  printf("   delete <key>\n");
  printf("\n");

  while (1) {
    ret = get_command();
    if (ret < 0) {
      printf("Error reading stdin\n");
      exit(-1);
    }

    if (strncmp(read_buf, "get", 3) == 0) {
      int key_start = 4;
      int key_end   = find_end(key_start);
      int key_len   = key_end - key_start;
      uint32_t value_len;

      memcpy(key_buf, read_buf + key_start, key_len);
      key_buf[key_len] = '\0';

      printf("Getting %s\n", key_buf);

      ret = libtocksync_kv_get(key_buf, key_len, data_buf, DATA_LEN, &value_len);
      if (ret < 0) {
        printf("Could not get key: %i\n", ret);
      } else {
        printf("Got value: ");
        for (int i = 0; i < (int) value_len; i++) {
          printf("%c", data_buf[i]);
        }
        printf("\n");
      }

    } else if (strncmp(read_buf, "set", 3) == 0) {
      int key_start   = 4;
      int key_end     = find_end(key_start);
      int key_len     = key_end - key_start;
      int value_start = key_end + 1;
      if (value_start >= DATA_LEN) {
        printf("Error parsing key and value, key too long.\n");
        continue;
      }
      int value_end = find_end(value_start);
      int value_len = value_end - value_start;

      memcpy(key_buf, read_buf + key_start, key_len);
      key_buf[key_len] = '\0';

      memcpy(value_buf, read_buf + value_start, value_len);
      value_buf[value_len] = '\0';

      printf("Setting %s=%s\n", key_buf, value_buf);

      ret = libtocksync_kv_set(key_buf, key_len, value_buf, value_len);
      if (ret < 0) {
        printf("Error setting key %i\n", ret);
      } else {
        printf("Set key-value\n");
      }

    } else if (strncmp(read_buf, "add", 3) == 0) {
      int key_start   = 4;
      int key_end     = find_end(key_start);
      int key_len     = key_end - key_start;
      int value_start = key_end + 1;
      if (value_start >= DATA_LEN) {
        printf("Error parsing key and value, key too long.\n");
        continue;
      }
      int value_end = find_end(value_start);
      int value_len = value_end - value_start;

      memcpy(key_buf, read_buf + key_start, key_len);
      key_buf[key_len] = '\0';

      memcpy(value_buf, read_buf + value_start, value_len);
      value_buf[value_len] = '\0';

      printf("Adding %s=%s\n", key_buf, value_buf);

      ret = libtocksync_kv_add(key_buf, key_len, value_buf, value_len);
      if (ret < 0) {
        printf("Error adding key %i\n", ret);
      } else {
        printf("Added key-value\n");
      }

    } else if (strncmp(read_buf, "update", 6) == 0) {
      int key_start   = 7;
      int key_end     = find_end(key_start);
      int key_len     = key_end - key_start;
      int value_start = key_end + 1;
      if (value_start >= DATA_LEN) {
        printf("Error parsing key and value, key too long.\n");
        continue;
      }
      int value_end = find_end(value_start);
      int value_len = value_end - value_start;

      memcpy(key_buf, read_buf + key_start, key_len);
      key_buf[key_len] = '\0';

      memcpy(value_buf, read_buf + value_start, value_len);
      value_buf[value_len] = '\0';

      printf("Updating %s=%s\n", key_buf, value_buf);

      ret = libtocksync_kv_update(key_buf, key_len, value_buf, value_len);
      if (ret < 0) {
        printf("Error updating key %i\n", ret);
      } else {
        printf("Updated key-value\n");
      }

    } else if (strncmp(read_buf, "delete", 6) == 0) {
      int key_start = 7;
      int key_end   = find_end(key_start);
      int key_len   = key_end - key_start;

      memcpy(key_buf, read_buf + key_start, key_len);
      key_buf[key_len] = '\0';

      printf("Deleting %s\n", key_buf);

      ret = libtocksync_kv_delete(key_buf, key_len);
      if (ret < 0) {
        printf("Could not delete key: %i\n", ret);
      } else {
        printf("Deleted key\n");
      }

    } else {
      printf("KV: unknown command\n");
    }
  }

  return 0;
}
