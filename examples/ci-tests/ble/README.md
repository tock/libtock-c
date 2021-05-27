# Hardware CI Tests

## Run Python Test

`gpio_test.py` provides basic tests that polls the pin output every 500 
milliseconds, and it monitors whether a toggle of the pin has occurred. If it
has occurred, it records the period to toggle the pin. The expected period 
should be around 1 seconds with a margin of error within 1 millisecond.

To run the test,
```bash
sudo python3 gpio_test.py Nrf52840GpioTest
```

Switch board name to the test you intend to run. Otherwise, python unittest
will attempt to run all tests.

Board | Test Name
------|----------
nrf52840dk | Nrf52840GpioTest
