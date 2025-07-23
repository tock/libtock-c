## Thread Tutorials

There are two Thread tutorials:
- Encrypted Sensor Data
- Temperature Sensor

Both tutorials introduce using Tock and OpenThread in addition
to presenting various components of Tock's design. They are
both intended as standalone seperate tutorials (although there
is some overlap in presenting the OpenThread concepts). The guide
for these tutorials can be found at:

- https://book.tockos.org/course/thread-tutorials/overview.html

### Encrypted Sensor Data Tutorial

This tutorial teaches participants how to create an OpenThread 
app (using Tock's OpenThread port) that is able to join a Thread
network and send/receive UDP packets. This tutorial focuses on the
ways in which Tock's can be used as a secure root of trust and how
this can be used to create secure, networked IoT devices.

### Temperature Sensor

This tutorial teaches participants how to create an OpenThread 
app (using Tock's OpenThread port) that is able to join a Thread
network and send/receive UDP packets. This tutorial guides participants
in creating a networked temperature sensor that includes user button
input, reading from a sensor, and displaying information on a screen.
This introduces concepts in Tock such as syscalls, callbacks, and 
interprocess communication. This tutorial also presents how Tock can
uniquely be used to create robust applications by leveraging Tock's
by-default process isolation.
