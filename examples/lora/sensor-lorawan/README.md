Sensor LoRaWAN Transmitter
==========================

This example builds an application to transmit sensor data via LoRaWAN.

See https://github.com/jgromes/RadioLib/blob/master/examples/LoRaWAN/LoRaWAN_Starter/notes.md
for notes on setting up the LoRaWAN device.

The most important part is setting the secrets from your LoRaWAN server
and any country specific settings.

To set the secrets first run the `lorawan-set-keys` example. That will set
the keys in flash. Then everytime you run the sensor-lorwan application
it will use those secrets.

This has been tested against The Things Network. Before changing settings
make sure you consider regulatory duty cycles and TTN's Fair Usage Policy,
for example don't drop the delay in the loop, otherwise you will break the
TTN Fair Usage Policy.

## Example Output

The output should look something like this.

```
Apollo3 chip revision: B
Initialization complete. Entering main loop
[SX1261] Initialising Radio ...
success!
[SX1261] Transmitting
success!
Waiting 60 minutes before transmitting again
```
