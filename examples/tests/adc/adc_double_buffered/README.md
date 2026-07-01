ADC Double Buffered Test App
============================

Demonstrates asynchronous continuous collection of analog samples in Tock.
Manually provides buffers and callbacks to the ADC driver and then begins
continuously sampling at 44.1 kHz. The provided buffers are 4410 Bytes in
length, so a callback occurs every 100 milliseconds.

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
[Tock] ADC Double Buffered Test
ADC driver exists with 6 channels
Beginning buffered sampling on channel 0 at 44100 Hz
Channel: 0	Count: 2205	Avg: 50037	Min: 49060	Max: 50878
Channel: 0	Count: 2205	Avg: 50036	Min: 49176	Max: 50917
Channel: 0	Count: 2205	Avg: 50039	Min: 49112	Max: 50788
Channel: 0	Count: 2205	Avg: 50028	Min: 49176	Max: 50969
Channel: 0	Count: 2205	Avg: 50033	Min: 49138	Max: 50827
Channel: 0	Count: 2205	Avg: 50035	Min: 49254	Max: 50865
Channel: 0	Count: 2205	Avg: 50033	Min: 49305	Max: 50981
Channel: 0	Count: 2205	Avg: 50042	Min: 49280	Max: 50827
Channel: 0	Count: 2205	Avg: 50037	Min: 49202	Max: 50865
Channel: 0	Count: 2205	Avg: 50037	Min: 49112	Max: 50878
Channel: 0	Count: 2205	Avg: 50038	Min: 49047	Max: 50917
```
