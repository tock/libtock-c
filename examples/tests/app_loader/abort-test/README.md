# Dynamic App Loader Abort Test (Helper App)
============================================
This app showcases the ability of the `abort` functionality of 
the Dynamic Process Loader.

When the app boots, it tries to load the `adc` app, aborts midway
and logs the result. Ideally, we should see:
`[Success] Abort Successful.`, and the `abort-test` app should 
terminate.