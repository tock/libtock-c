Hardware Root of Trust (HWRoT) Demo Encryption Service Application
------------------------------------------------------------------

This application implements a basic UART encryption service for a mock hardware root of trust (HWRoT) which inputs 

This is part of a tutorial which improves the application in multiple steps. A
writeup for the tutorial is available at https://book.tockos.org/.

Specific abilities of this version:

* Allows logging to screen over IPC
* Allows prompting for plaintext over returning results in hex over UART
* Allows for encryping user-provided plaintext using an encryption oracle driver
