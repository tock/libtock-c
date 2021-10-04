# GPIO Test

## Test Description

`test.py` provides the following tests:

### GpioTest

This test polls the pin output every 500 milliseconds of the RPi, and it monitors whether a toggle of the pin has occurred. If it has occurred, it records the period to toggle the pin. The expected period should be around 1 seconds with a margin of error within 1 millisecond.

  1. The test harness toggles the pin output of GPIO pins 5, 6, 13, 19, 26 of the RPi that is connected to the device-under-test every second.
  1. The device-under-test then has the connected GPIO pins toggled, where it's then recorded on the test harness and expected to change output every second with only a 1 millisecond margin of error.
  
## Example Output
     0.000186 INFO -- Initiating GPIO test...
     0.009582 INFO -- Setting up for nrf52840dk GPIO test...
     0.012508 INFO -- P0 state: high
     0.513447 INFO -- P0 state: low
     1.014629 INFO -- P0 state: low
     1.515798 INFO -- P0 state: high
     1.516388 INFO -- P0 state toggled in 1.0024113655090332s
     2.017339 INFO -- P0 state: high
     2.518495 INFO -- P0 state: low
     2.519038 INFO -- P0 state toggled in 1.002655029296875s
     3.019976 INFO -- P0 state: low
     3.521224 INFO -- P0 state: high
     3.521759 INFO -- P0 state toggled in 1.002718210220337s
     4.022839 INFO -- P1 state: high
     4.524027 INFO -- P1 state: low
     5.025043 INFO -- P1 state: low
     5.526238 INFO -- P1 state: high
     5.526565 INFO -- P1 state toggled in 1.002182960510254s

## Notes

When manually invoking the test harness, you must specify the board under test.
Otherwise, python unittest will attempt to run tests for all platforms.

To run the test,
```bash
sudo python3 test.py Nrf52840Test
```


### Supported Boards

CI has been validated and runs on the following hardware platforms:

Board | Test Name
------|----------
nrf52840dk | Nrf52840Test
hail | HailTest
