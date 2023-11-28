# Geolocation GNSS example application

## Description

This example illustrates the GNSS scan procedure:

- configuration of the LoRa Basics Modem library; and
- execution of GNSS *scan & send* feature using the *GNSS geolocation middleware*.

## GNSS middleware

The GNSS middleware is a software layer used to simplify the integration of a
GNSS scan & send operation in a user application.

For more details about the middleware, please refer to the documentation here:

[Geolocation middleware documentation](<../../../geolocation_middleware/readme.md>)

## LoRa Basics Modem configuration

Once the modem has reset, the application will set the ALMANAC_STATUS info field
in order to allow automatic OTA update of the almanac written in the LR11xx
memory.

Once the modem has joined the LoRaWAN network, the application will start the
time synchronization service.

Once the clock synchronization is done, the application will:
- configure the ADR custom profile for the selected region;
- disable the modem auto-switch to network controlled ADR;
- initialize the GNSS middleware; and
- initiate the first GNSS scan & send.

## GNSS middleware events

After the initial scan has been initiated, the application relies on the events
sent by the GNSS middleware to progress in the scan sequence.

For this, the application defines a callback function `on_middleware_gnss_event()`
that will be called when the GNSS middleware sends an event.

In this callback function, the application will process the pending events, and
for any event except the `GNSS_MW_EVENT_SCAN_DONE` event, it will schedule the
next scan with the delay defined by `GNSS_SCAN_GROUP_PERIOD`.

After all pending events have been processed, the function has to clear the
middleware pending events by calling the API function `gnss_mw_clear_pending_events()`.

## Assistance position

The application proposes 2 different modes for the assistance position:

* auto: when `MODEM_EXAMPLE_ASSISTANCE_POSITION_AUTO` is set to `true`, the
GNSS middleware will execute autonomous scans until the solver sends a
applicative downlink with an assistance position. The application will receive
the downlink on port `GNSS_DAS_DOWNLINK_PORT` and push the received payload to
the GNSS middleware (see `on_modem_down_data()`). After this, the middleware
will automatically switch to assisted scan.
*WARNING*: this mode is to be used when the object has a clear sky view, as the
sensitivity for autonomous scan is not as good as for assisted scan.
* user defined: when `MODEM_EXAMPLE_ASSISTANCE_POSITION_AUTO` is set to false,
the application will use the values defined in `MODEM_EXAMPLE_ASSISTANCE_POSITION_LAT`
and `MODEM_EXAMPLE_ASSISTANCE_POSITION_LONG` to configure the assistance
position to be used. In this mode, the GNSS middleware will only use assisted
scan.

## Limitations

It is expected that the almanac written in the LR11xx flash memory is up-to-date
enough. It is recommended to perform a full update before using this application.
No particular action is taken in case the GNSS middleware detects that the
almanac is too old to perform a successful scan, except sending an error event
`GNSS_MW_EVENT_ERROR_ALMANAC_UPDATE` and programming the next scan.

## Configuration

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

When compiling with *arm-none-eabi-gcc* toolchain, all these constant are configurable through command line with the `EXTRAFLAGS`.
See [README.md](../../../README.md#command-line-configuration).

### GNSS demonstration related configuration

The `main_geolocation_gnss.h` header file defines several constants to configure
geolocation parameters.

| Constant                                 | Comments                                                                                | Possible values  | Default Value |
| ---------------------------------------- | --------------------------------------------------------------------------------------- | ---------------- | ------------- |
| `MODEM_EXAMPLE_ASSISTANCE_POSITION_AUTO` | If set to `true`: configures the application to autonomously get an assistance position | {`true`,`false`} | `true`        |

If manual mode is selected for assistance position, the following constants
define the position to be used.

| Constant                                 | Comments                                                                        | Possible values            | Default Value      |
| ---------------------------------------- | ------------------------------------------------------------------------------- | -------------------------- | ------------------ |
| `MODEM_EXAMPLE_ASSISTANCE_POSITION_LAT`  | The latitude to use for GNSS Assisted scan (decimal degree)                     | Any `float` in [-90, 90]   | 45.181454          |
| `MODEM_EXAMPLE_ASSISTANCE_POSITION_LONG` | The longitude to use for GNSS Assisted scan (decimal degree)                    | Any `float` in [-180, 180] | 5.720893           |
| `MODEM_EXAMPLE_ASSISTANCE_POSITION_TEXT` | A text representation of the assistance position, for information printing only | Any constant c-string      | "Grenoble, FRANCE" |

*WARNING*: The pre-defined assistance position must be within a 150 km range of
the actual location.

| Constant                 | Comments                                                                                      | Possible values           | Default Value         |
| ------------------------ | --------------------------------------------------------------------------------------------- | ------------------------- | --------------------- |
| `GNSS_SCAN_GROUP_PERIOD` | Defines the duration between the end of a scan & send sequence and the start of next sequence | `uint32_t`                | 30                    |
| `GNSS_SCAN_MODE`         | Defines the GNSS scan mode (static or mobile) to be used for scan & send sequences.           | Value in `gnss_mw_mode_t` | `GNSS_MW_MODE_STATIC` |

The GNSS scan mode selected by default is `GNSS_MW_MODE_STATIC`, meaning that
this application example targets non-mobile objects.

When compiling with *arm-none-eabi-gcc* toolchain, all these constant are configurable through command line with the `EXTRAFLAGS`.
See [README.md](../../../README.md#command-line-configuration).

## Build

The demo can be built through GNU make command by doing the following:

```shell
# Navigate to the build folder
$ cd apps/geolocation_gnss/makefile

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

This application needs an Application Server to run in order to perform the GNSS
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
`main_geolocation_gnss.h` header file.

| Constant                | Comments                                                 |
| ----------------------- | -------------------------------------------------------- |
| `CUSTOM_NB_TRANS_EU868` | The number of retransmission to be used for EU868 region |
| `ADR_CUSTOM_LIST_EU868` | The custom ADR list to be used for EU868 region          |
| `CUSTOM_NB_TRANS_US915` | The number of retransmission to be used for US915 region |
| `ADR_CUSTOM_LIST_US915` | The custom ADR list to be used for US915 region          |
| `CUSTOM_NB_TRANS_CN470` | The number of retransmission to be used for CN470 region |
| `ADR_CUSTOM_LIST_CN470` | The custom ADR list to be used for CN470 region          |

The values must be carefully set to match with duty cycle constraints, power
consumption targets etc...

## Expected Behavior

Here follow the steps that shall be seen in the logs to indicate the expected behavior of the application.

### Device starts and resets

```
INFO: Modem Initialization

INFO: ===== LoRa Basics Modem GNSS Geolocation example (for static objects) =====
```

Following this print you shall find application and parameter prints

### Joined the network

At first run no time is supposed to be available

```
INFO: ###### ===== JOINED EVENT ==== ######
```

### Obtain time synchronization

```
INFO: ###### ===== TIME EVENT ==== ######
Time: SMTC_MODEM_EVENT_TIME_VALID
```

### Execute and send first autonomous GNSS scan

```
INFO: Set ADR custom profile for EU868
INFO: Initializing GNSS middleware v0.1.0
New scan group for autonomous scan
INFO: RP_TASK_GNSS - new scan group - task queued at 79363 + 0
---- internal scan start ----

INFO: ###### ===== MIDDLEWARE_1 EVENT ==== ######
INFO: GNSS middleware event - SCAN DONE
SCAN_DONE info:
-- token: 0x00
-- is_valid: 1
-- number of valid scans: 1
-- scan[0][1335796447] (6 SV): 222DEA586B20F0E0236AEB27B4371A419B68151226FA61D29F7CBC9706
   SV_ID 26:	45dB
   SV_ID 15:	45dB
   SV_ID 7:	43dB
   SV_ID 20:	43dB
   SV_ID 6:	42dB
   SV_ID 9:	40dB
-- power consumption: 7 uah
-- mode: 0
-- assisted: 0
-- almanac CRC: 0X6A820509

---- internal TX DONE ----
INFO: ###### ===== MIDDLEWARE_1 EVENT ==== ######
INFO: GNSS middleware event - TERMINATED
TERMINATED info:
-- number of scans sent: 1
```

### Receiving Assistance position

```
INFO: Received D-GNSSLOC-AIDP solver message
```

### Execute and send the assisted multiframe GNSS scan

```
New scan group for assisted scan
INFO: RP_TASK_GNSS - new scan group - task queued at 461083 + 30000
---- internal scan start ----
---- internal scan start ----
---- internal scan start ----
---- internal scan start ----
INFO: ###### ===== MIDDLEWARE_1 EVENT ==== ######
INFO: GNSS middleware event - SCAN DONE
SCAN_DONE info:
-- token: 0x07
-- is_valid: 1
-- number of valid scans: 4
-- scan[0][1335796859] (10 SV): 8247EA010A01567378AFF1C58A85C9DAAC54199BD49A4C2245D6E833409C1A89B0020B00
   SV_ID 26:	46dB
   SV_ID 7:	45dB
   SV_ID 15:	43dB
   SV_ID 6:	42dB
   SV_ID 9:	42dB
   SV_ID 89:	42dB
   SV_ID 20:	40dB
   SV_ID 99:	39dB
   SV_ID 92:	38dB
   SV_ID 108:	37dB
-- scan[1][1335796883] (10 SV): 8249EA010A01567338BBF07532486A9F4CD31D5CD41D3D2205571672D2625627B2CDCD0D
   SV_ID 26:	46dB
   SV_ID 7:	46dB
   SV_ID 15:	44dB
   SV_ID 92:	43dB
   SV_ID 9:	42dB
   SV_ID 89:	42dB
   SV_ID 98:	41dB
   SV_ID 6:	41dB
   SV_ID 108:	41dB
   SV_ID 20:	40dB
-- scan[2][1335796906] (10 SV): 824AEA010A01F6A071BEEEF559A82940EA143919D475192305076B32E7992F4C8B923308
   SV_ID 7:	46dB
   SV_ID 26:	46dB
   SV_ID 92:	45dB
   SV_ID 15:	42dB
   SV_ID 6:	42dB
   SV_ID 9:	42dB
   SV_ID 108:	40dB
   SV_ID 89:	40dB
   SV_ID 20:	40dB
   SV_ID 98:	40dB
-- scan[3][1335796930] (10 SV): 824CEA010A015473FACFEE65888DC97DB04CC19115EB34DB90F1A3E326897EC9D7095F0F
   SV_ID 26:	45dB
   SV_ID 92:	45dB
   SV_ID 7:	44dB
   SV_ID 15:	43dB
   SV_ID 6:	41dB
   SV_ID 89:	41dB
   SV_ID 108:	41dB
   SV_ID 99:	40dB
   SV_ID 98:	39dB
   SV_ID 93:	38dB
-- power consumption: 32 uah
-- mode: 0
-- assisted: 1
-- aiding position: (45.131836, 5.888672)
-- almanac CRC: 0X6A820509

# (...Several TX...)

---- internal TX DONE ----
INFO: ###### ===== MIDDLEWARE_1 EVENT ==== ######
INFO: GNSS middleware event - TERMINATED
TERMINATED info:
-- number of scans sent: 4
```