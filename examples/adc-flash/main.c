#include <stdio.h>
#include <timer.h>
#include <internal/nonvolatile_storage.h>
#include <adc.h>

uint8_t writebuf[256];

bool done = false;
bool timer_done = false;

static void write_done(int length,
                       __attribute__ ((unused)) int arg1,
                       __attribute__ ((unused)) int arg2,
                       __attribute__ ((unused)) void* ud) {
    done = true;
}

// callback for timers
static void timer_cb (__attribute__ ((unused)) int arg0,
                      __attribute__ ((unused)) int arg1,
                      __attribute__ ((unused)) int arg2,
                      __attribute__ ((unused)) void* userdata) {
    timer_done = true;
}

int main(void) {

    printf("Begin test\n");

    // Setup flash
    int ret = nonvolatile_storage_internal_write_buffer(writebuf, 256);
    if (ret != 0) printf("ERROR setting write buffer\n");
     
    ret = nonvolatile_storage_internal_write_done_subscribe(write_done, NULL);
    if (ret != 0) printf("ERROR setting write done callback\n");
    
    // Timer 
    tock_timer_t timer;
    timer_every(1000, timer_cb, NULL, &timer);

    uint8_t channel = 0;
    uint32_t freq = 300000;
    uint32_t length = 100;
    uint16_t buf[length];

    while(1){
        int err = adc_sample_buffer_sync(channel, freq, buf, length);

        // Output adc results
        if (err < 0) {
            printf("Error sampling ADC: %d\n", err);
        }
        else {
            //printf("\t[ ");
            for (uint32_t i = 0; i < length; i++) {
                // convert to millivolts
                writebuf[i] = (buf[i] * 3300) / 4095;
               // printf("%u ", writebuf[i]);
            }
            //printf("]\n ");
        }
        
        // Write to flash
        done = false;
        int ret  = nonvolatile_storage_internal_write(0, length);
        if (ret != 0) printf("ERROR calling write\n");
        yield_for(&done);

        yield_for(&timer_done);
        timer_done = false;
    }

}

