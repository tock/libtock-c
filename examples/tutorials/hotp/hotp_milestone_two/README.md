USB Security Key Application
----------------------------

This application implements a basic
[HOTP hardware token](https://en.wikipedia.org/wiki/HMAC-based_one-time_password)
which creates HOTP codes based on a secret key and outputs the codes over USB
HID as if it were a keyboard.

This is part of a tutorial which improves the application in multiple steps. A
writeup for the tutorial is available at https://book.tockos.org/

Specific abilities of this version:
 * Outputs an HOTP code over USB HID when button is pressed
 * Can program a new key on button hold
 * Saves key in Flash across reboots
 * Does NOT support multiple keys
