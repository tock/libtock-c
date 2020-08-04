#include "proximity.h"
#include "tock.h"

struct data {
    bool fired;
    int proximity;
};

static struct data result = { .fired = false };

// Internal callback for faking synchronous reads
static void cb(int proximity,
               __attribute__ ((unused)) int unused,
               __attribute__ ((unused)) int unused1,
               void* ud) {
  struct data* data = (struct data*) ud;
  data->proximity = proximity;
  data->fired    = true;
}


int proximity_set_callback(subscribe_cb callback , void* callback_args){
    return subscribe(DRIVER_NUM_PROXIMITY , 0 , callback , callback_args);
}

int proximity_read(){
    return command(DRIVER_NUM_PROXIMITY , 1 , 0 , 0);
}

int proximity_read_on_interrupt(){
    return command(DRIVER_NUM_PROXIMITY , 2 , 0 , 0);
}

int proximity_set_gain(uint8_t gain){
    return command(DRIVER_NUM_PROXIMITY , 3 , gain , 0);
}

int proximity_set_interrupt_thresholds(uint8_t low , uint8_t high){
    return command(DRIVER_NUM_PROXIMITY , 4 , low , high);
}

int proximity_set_interrupt_thresholds_sync(uint8_t low , uint8_t high){

    int err;
    result.fired = false;

    err = proximity_set_callback(cb , (void*) &result);
    if (err < 0){ return err; }

    err = proximity_set_interrupt_thresholds(low , high);
    if (err < 0){ return err; }

    yield_for(&result.fired);

    return 0;

}

int proximity_set_gain_sync(uint8_t gain){

    int err;
    result.fired = false;

    err = proximity_set_callback(cb , (void*) &result);
    if (err < 0){ return err; }

    err = proximity_set_gain(gain);
    if (err < 0){ return err; }

    yield_for(&result.fired);

    return 0;

}


int proximity_read_sync(unsigned* proximity){

    int err;
    result.fired = false;

    err = proximity_set_callback(cb , (void*) &result);
    if (err < 0){ return err; }

    err = proximity_read();
    if (err < 0){ return err; }

    yield_for(&result.fired);

    *proximity = result.proximity;


    return 0;


}

int proximity_read_on_interrupt_sync(unsigned* proximity){

    int err;
    result.fired = false;

    err = proximity_set_callback(cb , (void*) &result);
    if (err < 0){ return err; }

    err = proximity_read_on_interrupt();
    if (err < 0){ return err; }

    yield_for(&result.fired);

    *proximity = result.proximity;

    return 0;


}


