# Hardware CI Tests

## Run Python Test

`test.py` provides basic test that sends message from slave to master, and master checks if message has been sent through a specific function. If function has been called with proper connection, then check if message sent is what's expected. If so, test passes. Otherwise, test will fail due to poor connection or exception thrown from function, or message sent was not the expected message. 

To run the test,
```bash
sudo python3 test.py Nrf52840Test
```

Switch board name to the test you intend to run. Otherwise, python unittest
will attempt to run all tests.

Board | Test Name
------|----------
nrf52840dk | Nrf52840Test

**NOTE**
This test requires the Raspberry Pi to be set as Master. Thus, you must use Broadcom Pins 2 & 3 (SDA & SCL resepectively) or GPIO pin 3 and 5. 

This test requires button activation on tested boards. Two jumpers are required for the activation of the reset button and user button on boards. Broadcom Pins on Raspberry PI used are pins 20 & 21 (GPIO Pins 38 & 40) which are used for user button and reset respectively.