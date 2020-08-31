#include "proximity.h"
#include "tock.h"
#include <stdio.h>

struct thresholds
{
    uint8_t lowerThresh;
    uint8_t higherThresh;
};

// structure to store threshold values to be sent to the driver
static struct thresholds threshes = {.lowerThresh = 0, .higherThresh = 175};

struct data
{
    bool fired;
    int proximity;
};

static struct data result = {.fired = false};

// Internal callback for faking synchronous reads
static void cb(int proximity,
               __attribute__((unused)) int unused,
               __attribute__((unused)) int unused1,
               void *ud)
{
    struct data *data = (struct data *)ud;
    data->proximity = proximity;
    data->fired = true;
}

int proximity_set_callback(subscribe_cb callback, void *callback_args)
{
    return subscribe(DRIVER_NUM_PROXIMITY, 0, callback, callback_args);
}

int proximity_read(void)
{
    return command(DRIVER_NUM_PROXIMITY, 1, 0, 0);
}

int proximity_read_on_interrupt(uint8_t lower, uint8_t upper)
{
    return command(DRIVER_NUM_PROXIMITY, 2, lower, upper);
}

int proximity_set_interrupt_thresholds(uint8_t lower, uint8_t upper)
{
    threshes.lowerThresh = lower;
    threshes.higherThresh = upper;
    return 0;
}

int proximity_read_sync(unsigned *proximity)
{

    int err;
    result.fired = false;

    err = proximity_set_callback(cb, (void *)&result);
    if (err < 0)
    {
        return err;
    }

    err = proximity_read();
    if (err < 0)
    {
        return err;
    }

    yield_for(&result.fired);

    *proximity = result.proximity;

    return 0;
}

int proximity_read_on_interrupt_sync(unsigned *proximity)
{

    int err;
    result.fired = false;

    err = proximity_set_callback(cb, (void *)&result);
    if (err < 0)
    {
        return err;
    }

    err = proximity_read_on_interrupt(threshes.lowerThresh, threshes.higherThresh);
    if (err < 0)
    {
        return err;
    }

    yield_for(&result.fired);

    *proximity = result.proximity;

    return 0;
}
