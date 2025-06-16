# Dynamic App Loader (Helper App)
=================================

This app waits upon the user to press one of two buttons on the device (if supported).

When Button 1 (Default user button on boards) is pressed, the app tried to load in the 
`blink` app.

When Button 2 is pressed, the app tries to load in the `adc` app.

There are three stages to this:

1. Setup Phase
2. Flash Phase
3. Load Phase

#### Setup Phase
During the setup phase, the application passes the size of the new app `(blink)/(adc)`'s 
binary to the app_loader capsule. 

The capsule returns with either a success or failure. 

On success, the app requests the app_loader capsule to flash the `blink/adc` app.

On Failure, the app exits logs the reason for failure and exits.

#### Flash Phase
The app sends the binary of the `blink/adc` app 512 bytes (this is the size of the shared 
buffer between the app and the capsule) at a time along with the offset.

The capsule checks that these values do not violate the bounds dictated by the kernel 
and then requests the kernel to flash the app. 

The kernel performs more stringent checking on the request to ensure that memory access
violations do not take place, and flashes the app.

The capsule then returns with either a success or failure. 

On success, the app requests the app_loader capsule to load the `blink/adc` app.

On Failure, the app exits logs the reason for failure and exits.

#### Load Phase
The app requests the capsule to load the new app. There are only two outcomes: 

1. The `blink/adc` app is loaded successfully and functions without requiring a restart.
2. The load process failed somewhere, and the app is erased.