# Dynamic App Loader (HW-CLI Test App)
======================================

This app installs a new `tock-welcomes-dpl` app upon boot. 
This new app prints `Tock now supports dynamic application installs and updates!` 
before terminating.

#### Outcomes:
The app requests the capsule to load the new app. There are only two outcomes: 

1. The `tock-welcomes-dpl` app is loaded successfully and functions without requiring a restart.
2. The load process failed somewhere, and the app is erased.