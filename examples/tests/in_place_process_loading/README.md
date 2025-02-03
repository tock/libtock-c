# In-Place Process Loader (Helper App)
=================================

This app waits upon the user to press button 1 on the device (if supported).

This app assumes that the user knows the address and size of the app they want to 
make a process out of in flash. When the button is pressed, the app communicates
with the InPlaceProcessLoader capsule to create a process out of the binary.

I tested it by writing blink app to 0x50000. Then I installed this app. Blink 
did not work because there is no padding and the linked list is not preserved.
However when I press the button 1 on my nrf52840dk, it fetches the binary and makes
a process out of it.