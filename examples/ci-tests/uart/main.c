#include <stdio.h>
#include <stdlib.h>
#include <console.h>
#include <string.h>
#include <timer.h>
#include <tock.h>

int main(void){

    // Send Greeting Message
    const char* message_sent = "cse145 is cool";
    putnstr(message_sent, size_t(strlen(message_sent)));
    delay_ms(3000);

    // Expected Receive Message
    char* message_received;
    size_t len = 9;
    getnstr(message_received, len);
    delay_ms(3000);

    // Send Acknowledgement
    const char* ack = "true";
    putnstr(ack, size_t(strlen(ack)));
    
    return 0;
}