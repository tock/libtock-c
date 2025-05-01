# Dynamic App Loader (Helper App)
=================================

This app waits upon the user to press one of two buttons on the device (if supported).

When Button 1 (Default user button on boards) is pressed, the app tried to load in the 
`blink` app.

When Button 2 is pressed, the app tries to load in the `adc` app.

When button 3 is pressed, the app requests kernel to abort. 
The userland app then waits for another button press from the
user.

Not pressing button 3 will write and load an app.