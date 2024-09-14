# Test Multiple Alarms (With Overflow)

This tests the virtual alarms available to userspace. It sets two
alarms, first one that overflows the alarm, such that it's expiration
is small in absolute value (but should shouldn't fire until after the
clock overflows) and one after 1s. When successful, the second (1s)
alarm should fire after 1 second, while the second alarm should fire
after the clock overflows (approximately 7 minutes if the clock is at
32kHz).

If the virtual alarm library is buggy, this test might fail by
scheduling the 1 second alarm _after_ the longer wrapping alarm, and
it won't fire immediately.
