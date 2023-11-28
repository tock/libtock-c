# Geolocation GNSS and Wi-Fi example application

## Description

This example illustrates the combination of GNSS and Wi-Fi scan procedures:

- configuration of the LoRa Basics Modem library; and
- concurrent execution of GNSS and Wi-Fi *scan & send* features using the
*GNSS geolocation middleware* and the *Wi-Fi geolocation middleware*.

## GNSS & Wi-Fi middlewares

The geolocation middlewares belongs to a software layer used to simplify the
integration of a GNSS and Wi-Fi scan & send operation in a user application.

For more details about the middleware, please refer to the documentation here:

[Geolocation middleware documentation](<../../../geolocation_middleware/readme.md>)

## LoRa Basics Modem configuration

Same as [geolocation_gnss](<../geolocation_gnss/README.md>)

## Geolocation scan sequence

This application starts with a GNSS *scan & send* sequence, and as soon as it is
completed, it performs a Wi-Fi *scan & send* sequence (with no delay).

Once the Wi-Fi sequence is completed, it schedules the next GNSS sequence with
`GNSS_SCAN_GROUP_PERIOD` delay.

It uses the events sent by the geolocation middlewares to handle the concurrent
execution of GNSS and Wi-Fi sequences.

Please refer to [geolocation_gnss](<../geolocation_gnss/README.md>) and
[geolocation_wifi](<../geolocation_wifi/README.md>) for more details.

## GNSS Assistance position

Same as [geolocation_gnss](<../geolocation_gnss/README.md>)

## Limitations

Same as [geolocation_gnss](<../geolocation_gnss/README.md>)

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

### Geolocation demonstration related configuration

The `main_geolocation_gnss_wifi.h` header file defines several constants to
configure geolocation parameters.

| Constant                                 | Comments                                                                                | Possible values  | Default Value |
| ---------------------------------------- | --------------------------------------------------------------------------------------- | ---------------- | ------------- |
| `MODEM_EXAMPLE_ASSISTANCE_POSITION_AUTO` | If set to `true`: configures the application to autonomously get an assistance position | {`true`,`false`} | `false`       |

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
| `GNSS_SCAN_MODE`         | Defines the GNSS scan mode (static or mobile) to be used for scan & send sequences.           | Value in `gnss_mw_mode_t` | `GNSS_MW_MODE_MOBILE` |

The GNSS scan mode selected by default is `GNSS_MW_MODE_MOBILE`, meaning that
this application example targets mobile objects.

When compiling with *arm-none-eabi-gcc* toolchain, all these constant are configurable through command line with the `EXTRAFLAGS`.
See [README.md](../../../README.md#command-line-configuration).

## Build

The demo can be built through GNU make command by doing the following:

```shell
# Navigate to the build folder
$ cd apps/geolocation_gnss_wifi/makefile

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

This application needs an Application Server to run in order to perform the GNSS and Wi-Fi
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
`main_geolocation_gnss_wifi.h` header file.

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

INFO: ===== LoRa Basics Modem GNSS/Wi-Fi Geolocation example =====
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

### Execute and send first assisted GNSS scan

```
INFO: Set ADR custom profile for EU868
INFO: Initializing GNSS middleware v0.1.0
INFO: Initializing Wi-Fi middleware v0.1.0
WARN: User defined assistance position has been set in Grenoble, FRANCE
New scan group for assisted scan
INFO: RP_TASK_GNSS - new scan group - task queued at 67363 + 0
---- internal scan start ----
---- internal scan start ----

INFO: ###### ===== MIDDLEWARE_1 EVENT ==== ######
INFO: GNSS middleware event - SCAN DONE
SCAN_DONE info:
-- token: 0x00
-- is_valid: 1
-- number of valid scans: 2
-- scan[0][1335859412] (10 SV): 828DF902020115F3419EA946F82DCFD0541F212495C26A6A6404B25231D4ECCDFBB2D50A
   SV_ID 24:	45dB
   SV_ID 31:	45dB
   SV_ID 21:	44dB
   SV_ID 86:	43dB
   SV_ID 28:	43dB
   SV_ID 83:	43dB
   SV_ID 30:	42dB
   SV_ID 100:	42dB
   SV_ID 106:	41dB
   SV_ID 95:	41dB
-- scan[1][1335859420] (10 SV): 828DF902020115F36BD0A956D42DCF5753DF0E219582C40A326F35FDC4A6CE45FFFA9808
   SV_ID 24:	46dB
   SV_ID 100:	45dB
   SV_ID 31:	44dB
   SV_ID 21:	44dB
   SV_ID 86:	44dB
   SV_ID 28:	43dB
   SV_ID 106:	42dB
   SV_ID 83:	42dB
   SV_ID 30:	42dB
   SV_ID 95:	41dB
-- power consumption: 14 uah
-- mode: 1
-- assisted: 1
-- aiding position: (45.175781, 5.712891)
-- almanac CRC: 0X6A820509

---- internal TX DONE ----
INFO: ###### ===== MIDDLEWARE_1 EVENT ==== ######
INFO: GNSS middleware event - TERMINATED
TERMINATED info:
-- number of scans sent: 2
```

### Execute and send first Wi-Fi scan

```
INFO: RP_TASK_WIFI - new scan - task queued at 358709 + 0
---- internal Wi-Fi scan start ----
INFO: start Wi-Fi scan

INFO: ###### ===== MIDDLEWARE_2 EVENT ==== ######
INFO: Wi-Fi middleware event - SCAN DONE
SCAN_DONE info:
-- number of results: 3
-- power consumption: 0 uah
-- Timestamp: 1335859699
64 70 02 D9 94 55  -- Channel: 1 -- Type: 1 -- RSSI: -76
A4 3E 51 11 BC 15  -- Channel: 1 -- Type: 1 -- RSSI: -93
30 7C B2 C8 52 26  -- Channel: 6 -- Type: 1 -- RSSI: -93

---- internal TX DONE ----
INFO: ###### ===== MIDDLEWARE_2 EVENT ==== ######
INFO: Wi-Fi middleware event - TERMINATED
TERMINATED info:
-- number of scans sent: 1
New scan group for assisted scan
INFO: RP_TASK_GNSS - new scan group - task queued at 369925 + 30000
```