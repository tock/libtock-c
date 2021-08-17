# Hardware CI Tests

## Run Python Test

`test.py` provides basic test that sends message from master to slave. Tests if slave receives message through proper connection with specific function. If function fails, and throws out exception, then test fails. Otherwise, test will pass if connection is satisfied. 

To run the test,
```bash
sudo python3 test.py Nrf52840GpioTest
```

Switch board name to the test you intend to run. Otherwise, python unittest
will attempt to run all tests.

Board | Test Name
------|----------
nrf52840dk | Nrf52840Test

**NOTE**
This test requires the Raspberry Pi to be set as Master. Thus, you must use Broadcom Pins 2 & 3 (SDA & SCL resepectively) or GPIO pin 3 and 5. 