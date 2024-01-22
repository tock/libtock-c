I2C USB Bridge
==============

This application bridges data to and from the serial console to the I2C bus.
This allows a PC to communicate with an I2C device via a USB port.

On a PC you can connect to the UART of the device running this application to
access the I2C bus. The Tock device will then act as a I2C master to interact
with I2C devices.

-----------------------       ---------------------------       ----------
|              |      |  USB  |     Tock       |  I2C   |  I2C  |  I2C   |
|  PC (Linux)  | UART | <---> | I2C USB Bridge | Master | <---> | Device |
|              |      |       |                |        |       |        |
-----------------------       ---------------------------       ----------

Command Format
--------------

The command format is designed to match the
[I2C Driver](https://i2cdriver.com/i2cdriver.pdf) format.

The format is as follows

```
    <r|w> <addr> <len> <data>
```

For example to read two bytes from address `0x23`

```
    r 0x23 2
```

To write `0x12, 0x56` to address `0x23`

```
    w 0x23 2 0x12,0x56
```
