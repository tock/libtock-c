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

    char m = (char) c;
    
    char message[3];
    
    message[0] = 'r';
    message[1] = m;
    message[2] = '\0';
    
    printf("\n");
    delay_ms(1000);
    
    putnstr(message, (strlen(message)));
    
    delay_ms(3000);
    /*if(strcmp(echo_message, "") != 0){
        printf("\n Finishing loop");
        break;
    }*/
    delay_ms(2000);
  }
  
}
