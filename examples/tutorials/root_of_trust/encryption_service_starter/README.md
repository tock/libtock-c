Hardware Root of Trust (HWRoT) Demo Encryption Service Application
------------------------------------------------------------------

This application implements a basic UART encryption service for a mock hardware
root of trust (HWRoT) which inputs user-provided 

This is part of a tutorial which improves the application in multiple steps. A
writeup for the tutorial is available at https://book.tockos.org/.

Specific abilities of this version:

* Does NOT allow logging to screen over IPC
* Does NOT allow for plaintext over returning results in hex over UART
* Does NOT allow for encryping user-provided plaintext using an encryption oracle driver

This application is essentially just a scaffold to build from while following the tutorial.
