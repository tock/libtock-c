# I2C Master RX Test

## Test Description

`test.py` provides the following tests:

### I2CMasterRxTest

This receives message from the device-under-test as I2C Master from the RPi as I2C Slave.

  1. The device-under-test configures itself as I2C master with address 0x40 and awaits button press.
  1. The test harness presses a button (GPIO toggle) on device-under-test to indicate the device-under-test to prepare interaction and receive message as master.
  1. The test harness issues an I2C write transaction of 16 bytes and awaits master to send back read data.
  1. The test harness validates that the data received from device-under-test and sent back to test harness matches the expected string sent `"Hello I'm Slave"`.

## Example Output

      0.000194 INFO -- Initiating I2C Master Rx Test...
      1.017061 INFO -- Setting up for nrf52840dk I2C Master Rx test...
      1.017617 INFO -- Sending Messages As Slave... 
      2.020165 INFO -- Initiating Transmission of Messages...
      4.521819 INFO -- Messsage Call Back From Master: bytearray(b"Hello I\'m Slave")
      6.014079 INFO -- Messsage Call Back From Master: bytearray(b"Hello I\'m Slave")
      7.507251 INFO -- Messsage Call Back From Master: bytearray(b"Hello I\'m Slave")
      8.999247 INFO -- Messsage Call Back From Master: bytearray(b"Hello I\'m Slave")
      10.492707 INFO -- Messsage Call Back From Master: bytearray(b"Hello I\'m Slave")
      11.984940 INFO -- Messsage Call Back From Master: bytearray(b"Hello I\'m Slave")
      13.477794 INFO -- Messsage Call Back From Master: bytearray(b"Hello I\'m Slave")
      14.020983 INFO -- Message Sent: Hello I'm Slave
      14.021415 INFO -- Message Called Back from Master: Hello I'm Slave
      16.025693 INFO -- Connection Satisfied.
      17.027202 INFO -- I2C Master Rx Test has ended.


      .
      ----------------------------------------------------------------------
      Ran 1 test in 17.011s

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

This test requires button activation on tested boards. Two jumpers are required for the activation of the reset button and user button on boards. Broadcom Pins on Raspberry PI used are pins 20 & 21 (GPIO Pins 38 & 40) which are used for user button and reset respectively.

This test utilizes the pigpio open source library, but the function utilized, `bsc_i2c` , has a delay when updating the slave buffer being sent to master. This delay, to be specific, is that it takes an i2c transaction between master and slave to update the slave buffer properly. This is a limitation of the RPi slave configuration, not the board being tested i2c configuration. To combat this, we utilize a dummy transaction that runs a i2c transaction of sending messages for 4 seconds to master from slave, then reopens a transaction (the transacton being tested) with the updated slave buffer properly timed. For more information about this delay and what exactly is happening to handle it, check the code `test.py` and the function `dummy_transaction`.
