Test Alarm Overflow
====================

This tests the libtock alarm implementation's ability to handle situations
where setting an alarm would overflow the underlying hardware counter.

The userspace `libtock_alarm_in_ms` function expects a number of milliseconds when a timer
should fire in the future. This argument is a `uint32_t` which means that an
alarm should be able to be set $2^{32}$ ms in the future (about 49 days). However,
`libtock_alarm_in_ms` calls to `libtock_alarm_at` which can only set alarms up to $2^{32}$ clock ticks
(only about 36 hours with a 32768Hz oscillator). `libtock_alarm_in_ms` handles the underlying
overflow of the alarm and makes the timer appear as if it can set longer timers.
Under the hood, it keeps track of how many times the alarm needs to overflow until
the target time is reached (how many $2^{32}$ tick intevals need to pass). Only after
the last alarm does it call the original callback provided to `libtock_alarm_in_ms` while
hiding the effects of the intermediate alarms that were scheduled.

## Example Output

To replicate the behavior of the following example output, modify the "MAX_TICKS"
macro in "libtock/services/alarm.c" to whatever the frequency of your board is. 
For example, if the frequency is 32768Hz and we set "MAX_TICKS" to 32768 we can simulate 
the counter overflowing after 1 second.
Once you have simulated your board overflowing after 1 second, you can set "TIMER_LEN_MS"
in the test `main.c` to have the timer fire after 2 seconds. This corresponds to 
the board "overflowing" 2 times before reaching the target time of 2 seconds.

```
tock$ Timer Fired!
        Expected timer to fire at 2257 ms
        Timer actually fired at 2258 ms
        Difference of 1 ms
```
