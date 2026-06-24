ADC Continuous Test App
=======================

Demonstrates asynchronous continuous collection of single analog samples in
Tock. Samples at 10 Hz, and gets a sample callback every 100 ms.

On each callback, the samples are converted to millivolts and statistics about
the data are collected (min, max, and average). Results are printed to the
console.

Note that the ADC is not virtualized and can currently only be used by a single
application. Results are undefined if multiple applications (such as this and
the `hail` app) attempt to use the ADC at once. All applications on the system
can be erased with `tockloader erase-apps`.

Example Output
--------------

```
Initialization complete. Entering main loop.
[Tock] ADC Continuous Test
ADC driver exists with 6 channels
Beginning continuous sampling on channel 0 at 10 Hz
Channel: 0	Value: 52800
Channel: 0	Value: 52800
Channel: 0	Value: 52774
Channel: 0	Value: 52800
Channel: 0	Value: 52800
Channel: 0	Value: 52800
Channel: 0	Value: 52800
Channel: 0	Value: 52800
Channel: 0	Value: 52800
Channel: 0	Value: 52800
Beginning continuous sampling on channel 0 at 10 Hz
Channel: 0	Value: 52800
Channel: 0	Value: 52800
Channel: 0	Value: 52800
Channel: 0	Value: 52800
Channel: 0	Value: 52800
Channel: 0	Value: 52800
Channel: 0	Value: 52800
Channel: 0	Value: 52800
Channel: 0	Value: 52800
Channel: 0	Value: 52800
```
