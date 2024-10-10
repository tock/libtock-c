# Test Multiple Alarms (Ensure previous overflowing alarms aren't ignore)

This tests the virtual alarms available to userspace. It sets three
alarms alarms. The first two overflow and should result in multiple
alarms under the hood, and a third a 10 second alarm started after the
second. The first alarm is set first, but expires second (1 second
after overflow vs 10ms after overflow). The third alarm should expire
last.

When successful, the first alarm should fire after a clock overflow +
1 second, while the third should fire after a clock overflow + 10ms +
10 seconds.

If the virtual alarm library is buggy, this test might fail by never
firing the first alarm or firing it after the third alarm
(specifically almost another clock overflow) because either alarm
insertion or virtual alarm upcall handing misses it after the second
alarm fires.

# Example Output

Correct:

(after an entire clock overflow, ~8 minutes)

```
1 10498816 10496827
2 86100992 86099200
```
