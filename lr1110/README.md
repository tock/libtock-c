lr1110 - LoRaWAN and Wi-Fi Scanning for Tock
===================================

This library provides user space support for the LR1110 LoRa chip on [Wio-WM1110 development board](https://github.com/Seeed-Studio/Seeed_Wio_WM1110_Dev_Board) within Tock. It enables the device to join LoRaWAN (The Things Network), perform Wi-Fi scanning, and transmit data online, which could be further used for indoor geolocation.


Using lr1110 in libtock-c
-----------------------

To use this library in an app, include this line in the app Makefile:

```make
EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/lr1110
```

To use lr1110 library in the main source file of an example app, include this line at its beginning:

```c
#include <lr1110/lr1110.h>
```


Compile the library manually
----------------------------

When building an app that uses lr1110, this library will automatically be compiled.

To compile manually (perhaps for testing), run the following lines.

```
cd libtock-c/lr1110
make -f Makefile.setup
make
```
