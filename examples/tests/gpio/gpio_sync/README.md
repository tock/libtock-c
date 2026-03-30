GPIO Sync Interrupt Test App
=============

Test the libtock-sync/peripherals/gpio synchronous interrupt
event functions.

Expected Output
---------------

```
[Test] GPIO Sync Interrupt
Jump GPIO pin 0 low to start test.
Then move the jumper to VCC
tock$
Pin 0 went high
Now jumper back to ground
Pin 0 went low
Now jumper back to high
Pin 0 went back high
Success! Test done
```
