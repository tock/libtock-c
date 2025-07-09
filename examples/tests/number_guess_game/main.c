#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <libtock-sync/interface/console.h>
#include <libtock-sync/peripherals/rng.h>

static int getch(void) {
  uint8_t buffer[1];
  int number_read;
  libtocksync_console_read(buffer, 1, &number_read);
  return buffer[0];
}

static int putnstr(const char* str, int len) {
  int number_written;
  libtocksync_console_read((uint8_t*) str, len, &number_written);
  return number_written;
}

static int get_number(void) {
  char buffer[10] = {0};
  int idx         = 0;

  // Read in a number until a new line
  while (1) {
    int c = getch();

    if (c == RETURNCODE_FAIL) {
      printf("\ngetch() failed!\n");
      return 0;
    } else {
      char in = c;

      if (in == 10 || in == 13) {
        printf("\n");

        // On a newline go ahead and parse the buffer and return the number.
        return atoi(buffer);
      } else if ((in >= 48 && in <= 57) || in == 45) {
        // If this is a valid number record it
        buffer[idx] = in;
        idx        += 1;

        // Echo back to the user
        putnstr(&in, 1);

        // If our buffer is full, quit
        if (idx >= 10) {
          printf("\n");
          return atoi(buffer);
        }
      }
    }
  }
}

int main(void) {
  returncode_t ret;
  printf("Welcome to the number guessing game!\n\n");

  // Get some random number
  uint16_t random;
  int count;
  ret = libtocksync_rng_get_random_bytes((uint8_t*) &random, 2, 2, &count);
  if (ret != RETURNCODE_SUCCESS || count != 2) {
    printf("Error getting random number! There is a bug in this game :(\n");
    return -1;
  }

  printf("OK I have selected a random number between 0 and 65535\n");
  printf("Enter your guesses, and I will let you know when you are correct!\n");

  while (1) {
    putnstr("Enter a number: ", 16);
    int number = get_number();

    if (number == random) {
      printf("Hooray! You guessed my number.\n");
      printf("You are very good at this.\n");
      break;
    } else if (number < random) {
      printf("Nope. You are too low\n");
    } else {
      printf("Not quite. Please choose a lower number.\n");
    }
  }

  printf("Thank you for playing the number guessing game.\n");
  return 0;
}
