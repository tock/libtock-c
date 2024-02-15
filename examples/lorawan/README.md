# LoRa Basics Modem LoRaWAN Class A/C example

## Description

The application will automatically starts a procedure to join a LoRaWAN network (see [configuration](../../apps/common/lorawan_key_config.h)).

Once a network is joined (i.e. when the corresponding event is triggered), uplinks are sent periodically. This periodic action is based on the LoRa Basics Modem alarm functionality. Each time the alarm-related event is triggered, the application requests an uplink.

The content of the uplink is the value read out from the charge counter by calling `smtc_modem_get_charge()`.

The application is also capable of displaying data and meta-data of a received downlink.

## Configuration

Several parameters can be updated in `main_lorawan.h` header file:

| Constant                   | Description                                                                   |
| -------------------------- | ----------------------------------------------------------------------------- |
| `LORAWAN_APP_PORT`         | LoRaWAN FPort used for the uplink messages                                    |
| `LORAWAN_CONFIRMED_MSG_ON` | Request a confirmation from the LNS that the uplink message has been received |
| `APP_TX_DUTYCYCLE`         | Delay in second between two uplinks                                           |
