# I2C Master TX Test

## Test Description

`test.py` provides the following tests:

### I2CMasterTxTest

This sends message from the device-under-test as I2C Master to the RPi as I2C Slave.

  1. The device-under-test configures itself as I2C master with address 0x40 and awaits button press.
  1. The test harness presses a button (GPIO toggle) on device-under-test to indicate the device-under-test to send a message under master.
  1. The test harness issues an I2C read transaction of 16 bytes and awaits master to send data.
  1. The test harness validates that the data sent (and read as an RPi slave) matches the expected string `"Hello Friend."`.

## Example Output

      0.000206 INFO -- Initiating I2C Master Tx Test...
      1.017333 INFO -- Setting up for nrf52840dk I2C Master Tx test...

      1.017843 INFO -- Receiving Message As Slave... 

      2.020480 INFO -- Initiating Reception of Messages...
      2.023139 INFO -- Messsage Received: bytearray(b'Hello friend.\n\x00')
      4.516399 INFO -- Messsage Received: bytearray(b'Hello friend.\n\x00')
      7.010561 INFO -- Messsage Received: bytearray(b'Hello friend.\n\x00')
      9.504037 INFO -- Messsage Received: bytearray(b'Hello friend.\n\x00')
      11.998659 INFO -- Messsage Received: bytearray(b'Hello friend.\n\x00')
      14.021153 INFO -- Expected Message: Hello friend.
      14.021562 INFO -- Message Received: Hello friend.
      16.024844 INFO -- Connection Satisfied.
      17.026352 INFO -- I2C Master Tx Test has ended.
      .
      ----------------------------------------------------------------------
      Ran 1 test in 18.056s

      OK 

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

### Test Configuration Notes

**NOTE**
This test requires the Raspberry Pi to be set as slave. Thus, you must use Broadcom Pins 10 & 11 (SDA & SCL resepectively) or GPIO pin 19 and 23. You must also download the open source python library "pigpio" which enables slave access on the Raspberry Pi. How to download the library is done here on **CI Hardware Documentation - Raspberry Pi setup**

This test requires button activation on tested boards. Two jumpers are required for the activation of the reset button and user button on boards. Broadcom Pins on Raspberry PI used are pins 20 & 21 (GPIO Pins 38 & 40) which are used for user button and reset respectively. The user button and reset button depend on board tested.

