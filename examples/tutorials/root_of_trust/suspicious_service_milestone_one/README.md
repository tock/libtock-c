Hardware Root of Trust (HWRoT) "Suspicious" Userspace Attack Application
------------------------------------------------------------------------

This application implements a basic SRAM dump attack on the encryption service
application in the Hardware Root of Trust demo.

This is part of a tutorial which improves the encryption application in multiple
steps and mounts multiple attacks on it. A writeup for the tutorial is available
at https://book.tockos.org/.

Specific abilities of this version (assuming `#define` constants set correctly):

* Allows dumping application's own SRAM
* Allows attempting dumping of the encryption service application's SRAM 
