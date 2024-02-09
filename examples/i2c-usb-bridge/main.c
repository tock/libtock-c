#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <interface/console.h>
#include <peripherals/i2c_master.h>

#define DATA_LEN 64

char command_buf[DATA_LEN];
uint8_t recieve_buf[DATA_LEN];
uint8_t send_buf[DATA_LEN];

static int get_command(void) {
  int idx = 0;
  memset(command_buf, 0, DATA_LEN);

  // Read in characters until a new line
  while (1) {
    int c = getch();

    if (c == RETURNCODE_FAIL) {
      printf("getch() failed!\n");

      return c;
    } else {
      putnstr((char*) &c, 1);

      if (c == '\n' || c == '\r') {
        return 0;
      } else {
        // If this is a valid number record it
        command_buf[idx] = c;
        idx += 1;

        // If our buffer is full, quit
        if (idx >= DATA_LEN) {
          return 0;
        }
      }
    }
  }
}

static void read_data(char *command) {
  char *op_string, *addr_string, *len_string;
  long address, len;
  int i;

  op_string = strtok(command, " ");
  if (op_string == NULL) {
    printf("Unable to determine operation\n");
    return;
  }

  addr_string = strtok(NULL, " ");
  if (addr_string == NULL) {
    printf("Unable to determine address\n");
    return;
  }

  len_string = strtok(NULL, " ");
  if (len_string == NULL) {
    printf("Unable to determine length\n");
    return;
  }

  address = strtol(addr_string, NULL, 16);
  len     = strtol(len_string, NULL, 16);

  printf("Reading %ld bytes from 0x%lx\n", len, address);

  i2c_master_read_sync(address, recieve_buf, len);

  for (i = 0; i < len; i++) {
    printf("%c ", recieve_buf[i]);
  }

  printf("Application Operation complete\n");
}

static void write_data(char *command) {
  char *op_string, *addr_string, *len_string;
  long address, len;
  int i;

  op_string = strtok(command, " ");
  if (op_string == NULL) {
    printf("Unable to determine operation\n");
    return;
  }

  addr_string = strtok(NULL, " ");
  if (addr_string == NULL) {
    printf("Unable to determine address\n");
    return;
  }

  len_string = strtok(NULL, " ");
  if (len_string == NULL) {
    printf("Unable to determine length\n");
    return;
  }

  address = strtol(addr_string, NULL, 16);
  len     = strtol(len_string, NULL, 16);

  if (len > DATA_LEN) {
    printf("The data is larger then the maximum buffer size\n");
    return;
  }

  printf("Writing %ld bytes to 0x%lx\n", len, address);

  for (i = 0; i < len; i++) {
    char *byte = strtok(NULL, ",");

    if (byte == NULL) {
      printf("Not enough data supplied\n");
      return;
    }

    long value = strtol(byte, NULL, 16);

    send_buf[i] = value;
  }

  for (i = 0; i < len; i++) {
    printf("%c ", send_buf[i]);
  }

  i2c_master_write_sync(address, send_buf, len);

  printf("Application Operation complete\n");
}

int main(void) {
  printf("I2C USB Bridge\n");

  while (1) {
    int ret = get_command();

    if (ret < 0) {
      printf("Error reading stdin\n");
      exit(-1);
    }

    if (strncmp(command_buf, "r", 1) == 0) {
      read_data(command_buf);
    } else if (strncmp(command_buf, "w", 1) == 0) {
      write_data(command_buf);
    } else {
      printf("Invalid command: %s\n", command_buf);
      printf("Check the app README for instructinos\n");
    }
  }

  return 0;
}
