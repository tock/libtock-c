# LR11xx full almanac update example

## Description

This application executes a full almanac update by using the LR11xx API.
It does not involve LoRaWAN communication to update the almanac.
For the almanac update through LoRaWAN, refer to the example *almanac_update*.

This example also provides a simple python script *get_full_almanac.py* that fetches almanac content from LoRa Cloud and generate a C header file that is compiled with the embedded binary.

**NOTE**: This example is only applicable to LR1110 / LR1120 chips.

## Usage

The full almanac update with this example is executed in two steps:

1. Generate an almanac C header file with the python script *get_full_almanac.py*;
2. Build the example code and flash the binary to the Nucleo board.

### Generation of almanac C header file

The python script usage to generate the almanac C header file can be obtained with:

```bash
$ python ./get_full_almanac.py --help
usage: get_full_almanac.py [-h] [-f OUTPUT_FILE] mgs_token

Companion software that generates almanac header file to be compiled for LR1110/LR1120 embedded full almanac update.

positional arguments:
  mgs_token             MGS LoRa Cloud token to use to fetch the almanac

optional arguments:
  -h, --help            show this help message and exit
  -f OUTPUT_FILE, --output_file OUTPUT_FILE
                        file that will contain the results
```

### Compile and flash the binary code

The example code expects the almanac C header file produced by *get_full_almanac.py* python script to be named `almanac.h`.

The binary can be produced as the other examples (refer to [README.md](../../../README.md) for the details)

## Expected Behavior

Here follow the steps that has to be seen in the logs to indicate the expected behavior of the application.

### Device starts

```
INFO: Modem Initialization

INFO: ===== LoRa Basics Modem full almanac update example =====
```

### Full update of the almanac

The successful completion of the full almanac update is indicated by:

```
INFO: Local almanac doesn't match LR11XX almanac -> start update
INFO: Almanac update succeeded
```
