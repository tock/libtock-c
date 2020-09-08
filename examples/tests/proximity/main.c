#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <tock.h>
#include <proximity.h>
#include <timer.h>
#include <led.h>



int main(void){

    printf("Proximity Sensor Test\n");

    // Check if driver/sensor is on the board
    if( driver_exists(DRIVER_NUM_PROXIMITY) ){

        printf("Driver exists\n");
    } else {
        printf("Driver does not exist\n");
        return -1;
    }
    
    // Check led count
    int numLeds = led_count();
    printf("Number of LEDs on the board: %d\n" , numLeds);

    // Blink LED lights in order with frequency proportional to proximity reading
    // Main Loop starts once proximity reading is above a certain threshold
    
    unsigned frequency = 1000;
    int period = 1000;

    proximity_set_interrupt_thresholds(0,175);
    proximity_read_on_interrupt_sync(&frequency);

    while (true){

        for (int led = 0; led < numLeds; led++){
            led_on(led);
            delay_ms(period / frequency);
            led_off(led);
        }

        if (proximity_read_sync(&frequency) < 0){
            printf("Could not read proximity");
            return -1;
        }

    }
    return 0;
}