#include <stdio.h>
#include <stdlib.h>
#include <console.h>
#include <string.h>
#include <timer.h>
#include <tock.h>

#define TARGET_INCOMING_MESSGAE "yeah"
char* message_received = "";
int main(void){
    while(true){
        // Send Greeting Message
        const char* message_sent = "cse145 is cool";
        putnstr(message_sent, (strlen(message_sent)));
        // delay_ms(3000);

        // Expected Receive Message
        
        size_t len = 4;
        getnstr(message_received, len);
        delay_ms(3000);
        // printf("hahaha\n");
        // printf("This is string received");
        // printf(message_received);
        // printf("%d", strcmp(message_received, ""));
        if(strcmp(message_received, "") != 0){
            break;
        }
        delay_ms(2000);
    }

    printf("left while loop\n");
    while(true){
        // Send Acknowledgement
        const char* ack = "true";
        putnstr(ack, (strlen(ack)));
    }

    return 0;
}