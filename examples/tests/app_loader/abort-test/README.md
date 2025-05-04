# Dynamic App Loader Abort Test (Helper App)
============================================

This app waits upon console input. It only recognizes three characters - 
'0', '1' and '2'

When the app receives '0', it installs `tock-dpl-hello`, a variant of
the `c-hello` program. 

When the app receives '1', it installs `blink`.

When the app receives '2', it begins installing `adc`, and midway 
through the install, it aborts the install. This app essentially 
showcases the ability of the `abort` functionality of the Dynamic
Process Loader.
