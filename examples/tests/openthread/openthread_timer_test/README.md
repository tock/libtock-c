# OpenThread Timer Test

Openthread implements logic to provide a global time counter. OpenThread must handle wrapping logic to do this. This test verifies that the timer is able to accurately count upwards for 10 minutes.

To provide a realistic test, the standard openthread app setup/configuration is used. However, the timer logic is tested rather than launching the thread network and executing the thread main loop.
