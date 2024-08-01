# Geolocation Wi-Fi example application

## Description

This application demonstrates the usage of the Wi-Fi geolocation middleware and
how the LoRa Basics Modem should be configured to meet the prerequisites for
This example illustrates the Wi-Fi scan procedure:

- configuration of the LoRa Basics Modem library; and
- execution of Wi-Fi *scan & send* feature using the *Wi-Fi geolocation middleware*.

## Wi-Fi middleware

The Wi-Fi middleware is a software layer used to simplify the integration of a
Wi-Fi scan & send operation in a user application.

For more details about the middleware, please refer to the documentation here:

[Geolocation middleware documentation](<../../../geolocation_middleware/readme.md>)

## LoRa Basics Modem configuration

There is no particular modem configuration prerequisite for Wi-Fi scanning.

Once the modem has joined the network, the application will configure the ADR
custom profile for the selected region, disable the modem auto-switch to network
controlled ADR and finally initialize the Wi-Fi middleware, and immediately
initiate the first Wi-Fi scan & send.

## Wi-Fi middleware events

After the initial scan has been programmed, the application relies on the events
sent by the Wi-Fi middleware to progress in the scan sequence.

For this, the application defines a callback function `on_middleware_wifi_event()`
that will be called when the Wi-Fi middleware sends an event.

In this callback function, the application will process the pending events, and
for any event except the `WIFI_MW_EVENT_SCAN_DONE` event, it will program the
next scan with the delay defined by `WIFI_SCAN_GROUP_PERIOD`.

## Configuration

When compiling with *arm-none-eabi-gcc* toolchain, all these constant are configurable through command line with the `EXTRAFLAGS`.
See [README.md](../../../README.md#command-line-configuration).

### LoRaWAN related configuration

The `apps/common/lorawan_key_config.h` header file defines several constants to
configure the LoRaWAN parameters (profile, region, keys).

| Constant         | Comments                         | Possible values      | Default Value              |
| ---------------- | -------------------------------- | -------------------- | -------------------------- |
| `LORAWAN_REGION` | Selects the regulatory region    | See here-below       | `SMTC_MODEM_REGION_EU_868` |
| `LORAWAN_CLASS`  | Selects the LoRaWAN class to use | `SMTC_MODEM_CLASS_A` | `SMTC_MODEM_CLASS_A`       |

Supported values for `LORAWAN_REGION`:

* `SMTC_MODEM_REGION_EU_868 (default)`
* `SMTC_MODEM_REGION_US_915`
* `SMTC_MODEM_REGION_CN_470_RP_1_0`

Supported values for `LORAWAN_CLASS`:

* `SMTC_MODEM_CLASS_A`

### Wi-Fi demonstration related configuration

The `main_geolocation_wifi.h` header file defines several constants which can be
set to define the configurable parameters of the application.

| Constant           | Comments                                                                                      | Possible values | Default Value |
| ------------------ | --------------------------------------------------------------------------------------------- | --------------- | ------------- |
| `WIFI_SCAN_PERIOD` | Defines the duration between the end of a scan & send sequence and the start of next sequence | `uint32_t`      | 30            |
## Build

The demo can be built through GNU make command by doing the following:

```shell
# Navigate to the build folder
$ cd apps/geolocation_wifi/makefile

# Execute the make call
$ make -j
```

By default, the demonstration is compiled to use the EUIs and Application key
defined in the file *apps/common/lorawan_key_config.h*.

## Usage

### Serial console

The application requires no user intervention after the static configuration
option have been set.

Information messages are displayed on the serial console, starting with the
DevEUI, AppEUI/JoinEUI and PIN that you might need in order to register your
device with the LoRa Cloud Device Join service.

### LoRaWAN Network Server / Application Server

This application needs an Application Server to run in order to perform the Wi-Fi
solving.
Two possibilities :
* A Node-Red application server is provided in folder *apps/geolocation_application_server*.
Refer to the readme in this folder for details about setup and usage of the
application server.
* Use the LoRaCloud Locator https://atk.loracloud.com/ which embed a complete integration of an application server and an associated dashboard. 

### ADR configuration

The Adaptive Data Rate (ADR) is configured in *Custom ADR profile* with datarate
distribution and number of repetition defined per regions.

For more details about ADR configuration for geolocation middleware usage,
please refer to this [Geolocation middleware documentation](<../../../geolocation_middleware/doc/geolocationMiddleware.rst>), section "LoRaWAN datarate considerations".

The actual datarate and number of retransmission values are defined in the
`main_geolocation_wifi.h` header file.

| Constant                | Comments                                                 | Default value                        |
| ----------------------- | -------------------------------------------------------- | ------------------------------------ |
| `CUSTOM_NB_TRANS_EU868` | The number of retransmission to be used for EU868 region | 1                                    |
| `ADR_CUSTOM_LIST_EU868` | The custom ADR list to be used for EU868 region          | '5' repeated 9 times, '4' x5, '3' x2 |
| `CUSTOM_NB_TRANS_US915` | The number of retransmission to be used for US915 region | 2                                    |
| `ADR_CUSTOM_LIST_US915` | The custom ADR list to be used for US915 region          | '5' repeated 9 times, '4' x5, '3' x2 |
| `CUSTOM_NB_TRANS_CN470` | The number of retransmission to be used for CN470 region | 2                                    |
| `ADR_CUSTOM_LIST_CN470` | The custom ADR list to be used for CN470 region          | '3' repeated 9 times, '2' x5, '1' x2 |

The values must be carefully set to match with duty cycle constraints, power
consumption targets etc...

## Expected Behavior

Here follow the steps that shall be seen in the logs to indicate the expected behavior of the application.

### Device starts and resets

```
INFO: Modem Initialization

INFO: ===== LoRa Basics Modem Geolocation Wi-Fi example =====
```

Following this print you shall find application and parameter prints

### Joined the network

At first run no time is supposed to be available

```
INFO: ###### ===== JOINED EVENT ==== ######
```

### Execute and send the Wi-Fi scan

```
INFO: RP_TASK_WIFI - new scan - task queued at 40097 + 30000
---- internal Wi-Fi scan start ----
INFO: start Wi-Fi scan
WARN: No time available.

INFO: ###### ===== MIDDLEWARE_2 EVENT ==== ######
INFO: Wi-Fi middleware event - SCAN DONE
SCAN_DONE info:
-- number of results: 3
-- power consumption: 0 uah
-- Timestamp: 0
64 70 02 D9 94 55  -- Channel: 1 -- Type: 1 -- RSSI: -78
3C 17 10 B7 CD 90  -- Channel: 6 -- Type: 1 -- RSSI: -88
74 B6 B6 42 B4 EB  -- Channel: 1 -- Type: 2 -- RSSI: -87

---- internal TX DONE ----
INFO: ###### ===== MIDDLEWARE_2 EVENT ==== ######
INFO: Wi-Fi middleware event - TERMINATED
TERMINATED info:
-- number of scans sent: 1
```