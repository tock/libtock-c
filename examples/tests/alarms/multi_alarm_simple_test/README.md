# Test Multiple Alarms (Simple)

This tests the virtual alarms available to userspace. It sets two
repeating alarms, at 500ms and 1s respectively, each prints the alarm
index (1 or 2), current tick and alarm expiration to the console. When
successful, both alarms should fire and alarm 1 should fire twice as
often as alarm 2.

## Example Output

Correct:

```sh
1 6316032 6314240
2 10512384 10510592
1 10512384 10511104
1 14722560 14720768
2 18903552 18901760
1 18919680 18917632
1 23116544 23114752
2 27294720 27292928
...
```

(Note that timestamps, the second and third column, and exact ordering of `1` and `2` will differ by execution)

Incorrect:

```sh
1 7254784 7252992
2 11451136 11449344
2 19842304 19840512
2 28233472 28231680
2 36624640 36622848
2 45015808 45014016
2 53406976 53405184
...
```

Note that only alarm `2` appears after the first `1`.
