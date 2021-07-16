#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <console.h>
#include <tock.h>
#include <timer.h>

char* echo_message = "";
int main(void) {
  
  while (1) {
    int c = getch();

    /*if (c == RETURNCODE_FAIL) {
      printf("\ngetch() failed!\n");
    } else {
      printf("\n%c\n", (char) c);
    }*/
    
    //delay_ms(3000);
    char m = (char) c;
    
    char message[3];
    
    message[0] = 'r';
    message[1] = m;
    message[2] = '\0';
    
    printf("\n");
    delay_ms(1000);
    
    putnstr(message, (strlen(message)));

    //size_t len = 4;
    //getnstr(echo_message, len);
    //printf("%s", echo_message);
    
    delay_ms(3000);
    /*if(strcmp(echo_message, "") != 0){
        printf("\n Finishing loop");
        break;
    }*/
    delay_ms(2000);
  }
  
  
  /*while(true){
    // Send Acknowledgement
    const char* ack = "true";
    putnstr(ack, (strlen(ack)));
  }
  return 0;*/
}
