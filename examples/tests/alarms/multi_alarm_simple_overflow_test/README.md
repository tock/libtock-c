# Test Multiple Alarms (With Overflow)

This tests the virtual alarms available to userspace. It sets two
alarms, first one that overflows the alarm, such that its expiration
is small in absolute value (but shouldn't fire until after the clock
overflows) and one after 1s. When successful, the second (1s) alarm
should fire after 1 second, while the first alarm should wait to fire
until after the clock overflows (approximately 7 minutes if the clock
is at 32kHz).

If the virtual alarm library is buggy, this test might fail by
scheduling the 1 second alarm _after_ the longer wrapping alarm, and
it won't fire immediately.

# Example Output

Correct:

```
2 10512380  10511329
1 3 1
```

Incorrect:

(after no output for an entire clock overflow, e.g. ~7 minutes)

```
1 3 1
2 10512341  10511329
```
