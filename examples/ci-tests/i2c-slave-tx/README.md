# I2C Slave TX Test

## Test Description

`test.py` provides the following tests:

### I2CSlaveTxTest

This sends message from the device-under-test as I2C slave to the RPi as I2C master.

  1. The device-under-test configures itself as I2C slave with address 0x41 and awaits button press.
  1. The test harness presses a button (GPIO toggle) on device-under-test to indicate impending read.
  1. The test harness issues an I2C read transaction of 16 bytes to address 0x41.
  1. The test harness validates that the data read matches the expected string `"Hello Friend.\n"`.

## Example Output

    [rpi01] $ python3 test.py Nrf52840Test

    0.000219 INFO -- Initiating I2C Slave Tx test...
    1.018423 INFO -- Setting up for nrf52840dk I2C Slave Tx test...
    
    1.018837 INFO -- Communicating with I2C Device to Receive Message: Hello friend.
    2.022001 INFO -- Message Received: Hello friend.
    3.023379 INFO -- Message Sent Successfully from Slave
    
    3.023735 INFO -- Connection Satisfied.
    4.024175 INFO -- I2C Communication Ended...
    6.026811 INFO -- I2C Slave Tx Test has ended.
    
    .
    ----------------------------------------------------------------------
    Ran 1 test in 6.010s
    
    OK


## Notes

When manually invoking the test harness, you must specify the board under test.
Otherwise, python unittest will attempt to run tests for all platforms.

### Supported Boards

CI has been validated and runs on the following hardware platforms:

Board | Test Name
------|----------
nrf52840dk | Nrf52840Test

### Test Configuration Notes

**NOTE:**
This test requires the Raspberry Pi to be set as Master. Thus, you must use
Broadcom Pins 2 & 3 (SDA & SCL resepectively) which are also labeled as GPIO
pin 3 and 5 on the RPI I/O header.

**NOTE:**
This test requires button activation on tested boards. Two jumpers are required
for the activation of the reset button and user button on boards. Broadcom Pins
on Raspberry PI used are pins 20 & 21 (GPIO Pins 38 & 40) which are used for
user button and reset respectively.
