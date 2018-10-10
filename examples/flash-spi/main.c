#include <stdio.h>
#include <timer.h>
#include <internal/nonvolatile_storage.h>
#include <console.h>

#define BUF_SIZE 512 
uint8_t writebuf[BUF_SIZE];
bool done = false;
bool timer_done = false;

static void read_done(int length,
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
    int ret = nonvolatile_storage_internal_read_buffer(writebuf, 512);
    if (ret != 0) printf("ERROR setting read buffer\n");
     
    ret = nonvolatile_storage_internal_read_done_subscribe(read_done, NULL);
    if (ret != 0) printf("ERROR setting read done callback\n");
    
    // Timer 
    tock_timer_t timer;
    timer_every(1000, timer_cb, NULL, &timer);


    while(1){
        // Read from flash
        done = false;
        int ret  = nonvolatile_storage_internal_read(0, 512);
        if (ret != 0) printf("ERROR calling read %d \n",ret);
        yield_for(&done);

        uint8_t max = 0;
        uint8_t min = 0;
        for(int i =0; i<BUF_SIZE; i++){
            if(writebuf[i] > max){
                max = writebuf[i];
            }
            if(writebuf[i] < min){
                min = writebuf[i];
            }
        }

        // 20 bytes
        printf("Max:%c and Min:%c val\n",max,min);

        yield_for(&timer_done);
        timer_done = false;
    }

}

