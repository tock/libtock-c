Sensor Receive
==============

This example builds an application to receiver sensor data.

## Example Output

The output should look something like this.

Note that if the receive example is started before the transmit you will get
-6 errors (RX_TIMEOUT) like below, as there is nothing to receive.

```
Apollo3 chip revision: B
Initialization complete. Entering main loop
[SX1261] Initialising Radio ...
success!
[SX1261] Receiving...
failed, code -6
failed, code -6
failed, code -6
failed, code -6
failed, code -6
failed, code -6
failed, code -6
failed, code -6
failed, code -6
failed, code -6
failed, code -6
failed, code -6
success!: Temp: 24047, Hum: 5966
success!: Temp: 24047, Hum: 5966
success!: Temp: 24047, Hum: 5966
success!: Temp: 24047, Hum: 5966
success!: Temp: 24047, Hum: 5966
```