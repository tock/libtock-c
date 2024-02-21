# LoRa Basics Modem common content

## Description

In order to bring some clarity, several generic operations (i.e. operations that are used no matter the LoRa Basics Modem functionality being demonstrated in an example) are implemented in this common folder.

## LoRaWAN configuration

In this SDK, the LoRaWAN configuration is handled by the function `apps_modem_common_configure_lorawan_params()`.

The join parameters (DevEUI, JoinEUI and AppKey) can be taken from different places:

* if `USER_DEFINED_JOIN_PARAMETERS` is defined: the join parameters are taken from [lorawan_key_config.h](./lorawan_key_config.h)
* if `LR11XX_DEFINED_JOIN_PARAMETERS` is defined: the join parameters are taken from the LR11XX and the application key is derived according to the algorithm described in LoRa Cloud.

In the user-defined case, the configuration has to be updated in [lorawan_key_config.h](./lorawan_key_config.h):

| Constant             | Description             | Possible values                             | Default value                                                                                      | Note                                              |
| -------------------- | ----------------------- | ------------------------------------------- | -------------------------------------------------------------------------------------------------- | ------------------------------------------------- |
| `LORAWAN_DEVICE_EUI` | LoRaWAN device EUI      | Any 8 bytes c-array                         | `{0xFE, 0xFF, 0xFF, 0xFF, 0xFD, 0xFF, 0x00, 0x00}`                                                 | Used if `USER_DEFINED_JOIN_PARAMETERS` is defined |
| `LORAWAN_JOIN_EUI`   | LoRaWAN join EUI        | Any 8 bytes c-array                         | `{0x00, 0x16, 0xC0, 0x01, 0xFF, 0xFE, 0x00, 0x01}`                                                 | Used if `USER_DEFINED_JOIN_PARAMETERS` is defined |
| `LORAWAN_APP_KEY`    | LoRaWAN application key | Any 16 bytes c-array                        | `{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}` | Used if `USER_DEFINED_JOIN_PARAMETERS` is defined |
| `LORAWAN_REGION`     | LoRaWAN region          | Values in `smtc_modem_region_t` enumeration | `SMTC_MODEM_REGION_EU_868`                                                                         | See supported LoRaWAN regions below               |
| `LORAWAN_CLASS`      | LoRaWAN class           | Values in `smtc_modem_class_t` enumeration  | `SMTC_MODEM_CLASS_A`                                                                               | All LoRaWAN classes (A, B & C) are supported      |

When compiling with *arm-none-eabi-gcc* toolchain, all these constant are configurable through command line with the `EXTRAFLAGS`.
See [README.md](../../../README.md#command-line-configuration).

The LoRaWAN regions supported are EU868, US915, CN470, AS923 (Australia, Japan, Indonesia), KR920, IN865, RU864, AU915.

## LoRa Basics Modem event management

When LoRa Basics Modem is initialized, a callback is given as parameter to `smtc_modem_init()` so the application can be informed of events. In a final application, it is  up to the user to implement this function.

In this SDK, this function is pre-defined to bring consistency - it is `apps_modem_event_process()` implemented in [apps_modem_event.c](./apps_modem_event.c). Each example needs to implement the functions defined in `apps_modem_event_callback_t` that are useful to it - others have to be set to `NULL`.
