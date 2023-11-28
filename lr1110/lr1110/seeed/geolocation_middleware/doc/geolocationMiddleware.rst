Geolocation middlewares
=======================

.. _Middleware Introduction:

Introduction
------------

The geolocation middlewares aim to provide a high level API to run "scan & send" sequences, as simply as possible.

The middlewares provide an abstraction layer:

* on top of the LR11xx radio for geolocation features (GNSS & Wi-Fi scanning)
* based upon the LoRa Basics Modem for scheduling operations and sending results.

The goal is to expose as few parameters as possible to the user application, to accomplish pre-defined use cases with the best performances.

The user application just has to program when the next scan needs to be launched. The middleware will notify when the operation has completed with events.

.. _fig_docMiddlewareSequenceOverview:

.. figure:: geoloc_docMiddlewareSequenceOverview.png
   :scale: 80%
   :align: center
   :alt: middleware scan sequence overview

   middleware scan & send sequence overview

.. _GNSS Middleware:

GNSS middleware
---------------

.. _GNSS scan group:

Scan group
++++++++++

In order to get the best accuracy from GNSS scan results, it is recommended to use the **multiframe** feature of the solver.

In this context, a scan group is a group of scan results (NAV messages) to be used together to solve a position.

A scan group also has an identifier (a token) used to identify the NAV messages which have to be grouped for multiframe solving by the Application Server.

A scan group can be *valid* if all scans within the group meets the expected criteria (minimum number of Space Vehicles detected...).
So it can also be *invalid*, in this case the results will not be sent over the air by the middleware.

As soon as a scan with the group doesn't meet the expected criteria, the scan group is aborted.

.. _GNSS scanning modes:

Scanning modes
++++++++++++++

Currently there are 2 modes supported, but this could be extended in the future.

* **STATIC mode**: to be used for non-moving objects
* **MOBILE mode**: to be used for mobile objects

Selecting a particular mode will indicate to the middleware how operations must be sequenced to get the best performances.

    **STATIC mode**

    When this mode is selected, with ``GNSS_MW_MODE_STATIC``, the GNSS middleware will run 4 GNSS scans, with a delay of 15 seconds between the end of a scan and the start of the next one.
    Once the last scan is completed, it will send each scan result with a LoRaWAN uplink, one after the other. The full scheme is shown on figure `fig_geoloc_staticScanScheme`_

    **MOBILE mode**

    When this mode is selected, with ``GNSS_MW_MODE_MOBILE``, the GNSS middleware will run 2 GNSS scans, with no delay between the end of a scan and the start of the next one.
    Once the last scan is completed, it will send each scan result with a LoRaWAN uplink, one after the other. The full scheme is shown on figure `fig_geoloc_mobileScanScheme`_


.. _fig_geoloc_staticScanScheme:

.. figure:: geoloc_staticScanScheme.png
   :align: center
   :alt: Scan scheme for a static mode

   Illustration of *STATIC mode* showing the scan sequence and transmission over LoRaWAN


.. _fig_geoloc_mobileScanScheme:

.. figure:: geoloc_mobileScanScheme.png
   :align: center
   :alt: Scan scheme for a mobile mode

   Illustration of *MOBILE mode* showing the scan sequence and transmission over LoRaWAN


.. _GNSS events notification:

Events notification
+++++++++++++++++++

In order to inform the user application about the "scan & send" sequence status, it will send several events to indicate what happened and allow the user application to take actions.

* **SCAN_DONE**: This event is always sent, at the end of the scan sequence (before sending results over the air). It is also sent if the scan group has been aborted, and set to invalid.
* **TERMINATED**: This event is always sent, at the end of the send sequence (even if nothing has been sent).
* **CANCELLED**: Sent when the middleware acknowledges a user cancel request.
* **ERROR_ALMANAC**: Sent when a scan could not be done due to an ALMANAC being too old.
* **ERROR_NO_TIME**: Sent when a scan could not be done due to no valid time available (clock sync).
* **ERROR_NO_AIDING_POSITION**: Sent when a scan could not be done due to no assistance position configured.
* **ERROR_UNKNOWN**: An unknown error occurred during the sequence.

When data associated with an event are available, a dedicated API function to retrieve those data is provided. It is the case for SCAN DONE event and TERMINATED event.

.. _GNSS internal choices:

Internal choices
++++++++++++++++

In order to reach an acceptable trade-off for performances, power consumption and simplicity, some parameters have been set in the middleware, and are not configurable from the API.

* A maximum of 10 Space Vehicles detected per NAV message: allow good accuracy while still being able to transmit a complete NAV message in 1 uplink (49 bytes when dopplers are enabled).
* LR1110 scan parameters: dopplers are enabled for autonomous scan only so that the doppler solver can be used to get an assistance position back.
* There are 2 scan group modes to choose the strategy for determining when a scan group is valid or not:

  * `GNSS_SCAN_GROUP_MODE_DEFAULT`: a scan group is valid if the number of valid scans is equal to the group size. As soon as a scan fails (not enough SV detected), the scan group is aborted and not sent over the air (for power consumption saving).
  * `GNSS_SCAN_GROUP_MODE_SENSITIVITY`: a scan group is valid as soon as there is a valid scan in the group (with a valid NAV message). This mode optimizes the chances to get a position even if the environment is not ideal. The position accuracy can be lower than with GNSS_SCAN_GROUP_MODE_DEFAULT mode because the position solving could be done with less data.

Some clarification about what is a valid scan group, a valid scan or a valid NAV message:

* *scan group*: a scan group is valid if the result of the function `gnss_scan_group_queue_is_valid()` is true, which depends on the scan group mode selected.
* *scan*: a scan is valid if the LR11xx radio returned a minimum number of detected space vehicles (the minimum number is defined by the scanning mode).
* *NAV message*: a NAV message is valid if the result of the function `smtc_gnss_is_nav_message_valid()` is true, which depends on the number of SV detected per constellation.

For example, in `GNSS_SCAN_GROUP_MODE_SENSITIVITY` scan group mode, a scan group is:

* *not valid* if there was only one valid scan with an invalid NAV message.
* *valid* if there were 2 valid scans, even if the individual NAV messages would be invalid (no check on individual NAV validity for multiframe solving).

.. _GNSS default options:

Default options
+++++++++++++++

We have made the choice to keep configuration parameters as low as possible for a standard usage of the middleware.

By default:

* The GNSS constellations used are: **GPS & BEIDOU**
* Each scan results is sent as a dedicated LoRaWAN uplink on **port 194**.
* The scan group token is incremented by 1 for each valid scan group.
* The scan group mode selected is `GNSS_SCAN_GROUP_MODE_SENSITIVITY`.

.. _GNSS advanced options:

Advanced options
++++++++++++++++

Some default parameters can be overruled for specific use cases:

* The constellations to be used: use GPS only, BEIDOU only
* The port on which the LoRaWAN uplink is sent
* The sequence can be set as "send_bypass" mode, meaning that the scan results won't be automatically sent by the middleware. It can be useful if the user application wants to send the result in a specific manner (using modem streaming feature...).
* Several scan groups can be aggregated together by keeping the same token. It can be useful for non-mobile objects for multiframe solving with a sliding window.

.. _GNSS assistance Position:

Assistance/Aiding Position
++++++++++++++++++++++++++

The best performances for GNSS geolocation is achieved by using the "assisted scan" feature of the LR1110 radio. In order to use this feature, the middleware needs to provide an assistance position to the radio.

There are 2 ways to provide this assistance position:

* an assistance position is given by the user at application startup.
* no assistance position is given by the user, so the middleware starts with an "autonomous scan" and rely on the solver and the application server to return an assistance position with an applicative downlink based on the autonomous can result.

Note: When using autonomous scan, the sensitivity is not optimal. A better sky view is required to detect Space Vehicles compared to assisted scan.
So it is recommended, if possible, to set an assistance position (as accurate as possible) at startup.

.. _Internals of the GNSS middleware:

Internals of the GNSS middleware
++++++++++++++++++++++++++++++++

The main role of the middleware is to ease the usage of the LR11xx radio and avoid conflicts between the radio usage for GNSS scanning and other concurrent tasks in an application (transmitting packets...).

For this, the middleware heavily relies on LoRa Basics Modem (LBM) and in particular its Radio Planner.

In the LBM, the Radio Planner is responsible for arbitrating the radio usage and ensure that only one user tries to access it at a time.

* So, when the user calls the ``gnss_mw_scan_start()`` function to start a GNSS scan in the specified delay, it basically schedules a new task in the Radio Planner. The task is scheduled with the ASAP mode, this means that if the radio is already busy at the requested time, the task will be shifted and executed As Soon As Possible.
* When the Radio Planner is ready to launch the programmed task, the ``gnss_mw_scan_rp_task_launch()`` function is called, and the LR11xx radio is ready to be configured to perform the first scan of the scan group. **It is important to note that the code is executed under interrupt, so it needs to be as quick to execute as possible.**
* Once the LR11xx radio has completed the scan, the Radio Planner will call the ``gnss_mw_scan_rp_task_done()`` function of the middleware. **Again, this function is executed under interrupt context, so needs to be fast.** This function will get the scan results and store it in the scan group queue. It will also send a ``GNSS_MW_EVENT_SCAN_DONE`` event to the application. The user application can retrieve scan results and statistics by calling the ``gnss_mw_get_event_data_scan_done()`` function.
* Then, either it is the last scan of the group and it will trigger the first transmission, or it is not the last and it will program the next scan of the queue.
* For sending results over the air, the middleware uses an extended internal API of the LBM which does not copy the buffer to be sent, so the middleware must ensure that the buffer to be sent keeps consistent until it is sent. The LBM will call the ``gnss_mw_tx_done_callback()`` for each completed transmission, and based on this, the middleware will pop all results to be sent over the air.
* Once the last scan result of the scan group has been sent, the ``GNSS_MW_EVENT_TERMINATED`` event is sent to the application, and the scan sequence is over.

.. _Prerequisites for a GNSS scan:

Prerequisites for a GNSS scan
+++++++++++++++++++++++++++++

There are some prerequisites necessary to have a functional GNSS scan, and to get the best performances. It is the responsability of the user application to ensure that those requirements are met.

* **time**: a valid time must be provided (ALC Sync, network clock sync...).
* **almanac**: the Almanac written in the LR11xx flash memory must be as up-to-date as possible. It can either be be fully updated at once, or incrementally updated through LoRaCloud Modem & Geolocation Services.
* **assistance position**: an assistance position must be provided to the middleware, either as a user defined assistance position, or by forwarding the downlink coming from the solver.

.. _GNSS scan results payload format:

Scan results payload format
+++++++++++++++++++++++++++

As the middleware automatically sends the scan results for location solving, it has control over the format used for the uplink.

The format is the following:

.. _table-gnss-payload:

.. table:: GNSS results payload format.

    +---------------------+------------------+--------------------+
    | scan group last NAV | scan group token | NAV message        |
    +=====================+==================+====================+
    | 1 bit               | 7 bits           | 36 or 49 bytes max |
    +---------------------+------------------+--------------------+

* scan group last NAV: indicates to the Application Server if this message is the last of a scan group. It can be used to trigger a multiframe solving request by the Application Server.
* scan group token: it is the identifier of the current scan group. It is used by the Application Server to group the NAV message which should be used as a multiframe solving request.
* NAV message: it is the GNSS scan result returned by the LR11xx radio. The actual size depends on the number of Space Vehicle detected by the scan, and if dopplers are enabled or not. For assisted scans, the maximum size is 49 bytes if dopplers are enabled, and 36 bytes otherwise.

The maximum size of the complete payload has been kept under 51 bytes to match with the maximum payload size allowed by the LoRaWAN Regional Parameters for most regions (there are few exceptions like DR0 of the US915 region which therefore cannot be used).

.. _LoRaWAN datarate considerations for GNSS:

LoRaWAN datarate considerations
+++++++++++++++++++++++++++++++

As seen in the section `GNSS scan results payload format`_ , due to the maximum length of the scan results payload, some LoRaWAN datarates cannot be used to transmit the results.

Also, depending on the region of operation and how often it is required to get a position for the final application, much care should be taken of the datarates used.

It is **mandatory** to disable the "Network Controlled" mode for Adaptative Datarate (ADR) and rather used custom profiles.
In this custom profiles, it is generally more efficient to use fast datarates, and increase the number of retransmission.

It is to be noted that the same ADR configuration will be used for sending geolocation scan results and application specific payloads.

.. _Cancelling a GNSS scan:

Cancelling a GNSS scan
++++++++++++++++++++++

The middleware API provides a function ``gnss_mw_scan_cancel()`` which can be used by the user application to cancel a programmed scan operation.

It is important to note that a scan can be cancelled only if the corresponding task has not yet been launched. A scan task which has been launched cannot be aborted and will complete (both scan and send).

A scan task is considered "launched" when the delay to start the scan has elapsed and the Radio Planner has granted access to the radio.

.. _GNSS API:

API
+++

Refer to the ``gnss/src/gnss_middleware.h`` file.

.. _Wi-Fi Middleware:

Wi-Fi middleware
----------------

Contrary to the GNSS middleware, there is no scan group concept in the Wi-Fi middleware, and no multiframe solving.
A Wi-Fi scan will simply return the list of Access Points MAC address that have been detected, and will be sent to the solver within one uplink message.

.. _Wi-Fi events notification:

Events notification
+++++++++++++++++++

In order to inform the user application about the "scan & send" sequence status, it will send several events to indicate what happened and allow the user application to take actions.

* **SCAN_DONE**: This event is always sent, at the end of the scan sequence (before sending results over the air). It is also sent if the scan group has been aborted, and set to invalid.
* **TERMINATED**: This event is always sent, at the end of the send sequence (even if nothing has been sent).
* **CANCELLED**: Sent when the middleware acknowledges a user cancel request.
* **ERROR_UNKNOWN**: An unknown error occurred during the sequence.

When data associated with an event are available, a dedicated API function to retrieve those data is provided. It is the case for SCAN DONE event and TERMINATED event.

.. _Wi-Fi internal choices:

Internal choices
++++++++++++++++

The following parameters are set by the middleware, and are not configurable from the API.

* A Minimum of 3 Access Points must be detected to get a valid scan.
* The scan will stop when a maximum of 5 Access Points have been detected.
* All channels are enabled to be scanned.
* A scan will look for Beacons of type B, G and N.
* The maximum time spent scanning a channel is set to 300ms
* The maximum time spent for preamble detection for each single scan is set to 90ms

*Note*: The current implementation is very basic, and does not provide the best performances possible in terms of accuracy and power consumption. It will be improved in further version.

.. _Wi-Fi default options:

Default options
+++++++++++++++

We have made the choice to keep configuration parameters as low as possible for a standard usage of the middleware.

By default:

* Each scan results is sent as a dedicated LoRaWAN uplink on **port 196**.

.. _Wi-Fi advanced options:

Advanced options
++++++++++++++++

Some default parameters can be overruled for specific use cases:

* The port on which the LoRaWAN uplink is sent
* The sequence can be set as "send_bypass" mode, meaning that the scan results won't be automatically sent by the middleware. It can be useful if the user application wants to send the result in a specific manner (using modem streaming feature...).

.. _Internals of the Wi-Fi middleware:

Internals of the Wi-Fi middleware
+++++++++++++++++++++++++++++++++

The main role of the middleware is to ease the usage of the LR11xx radio and avoid conflicts between the radio usage for GNSS scanning and other concurrent use for other tasks in an application (transmitting packets...).

For this, the middleware heavily relies on LoRa Basics Modem (LBM) and in particular its Radio Planner.

In the LBM, the Radio Planner is responsible for arbitrating the radio usage and ensure that only one user tries to access it at a time.

* So, when the user calls the ``wifi_mw_scan_start()`` function to start a Wi-Fi scan in the specified delay, it basically schedules a new task in the Radio Planner. The task is scheduled with the ASAP mode, this means that if the radio is already busy at the requested time, the task will be shifted and executed As Soon As Possible.
* When the Radio Planner is ready to launch the programmed task, the ``wifi_mw_scan_rp_task_launch()`` function is called, and the LR11xx radio is ready to be configured to perform the scan. **It is important to note that the code is executed under interrupt, so it needs to be as quick to execute as possible.**
* Once the LR11xx radio has completed the scan, the Radio Planner calls the ``wifi_mw_scan_rp_task_done()`` function of the middleware. **Again, this function is executed under interrupt context, so needs to be fast.** This function gets the scan results and store it in the middleware context. It also sends a ``WIFI_MW_EVENT_SCAN_DONE`` event to the application. The user application can retrieve scan results and statistics by calling the ``wifi_mw_get_event_data_scan_done()`` function.
* Then, the middleware sends the results over the air. For this, it uses an extended internal API of the LBM which does not copy the buffer to be sent, so the middleware must ensure that the buffer to be sent is kept consistent until it is sent. The LBM calls the ``wifi_mw_tx_done_callback()`` when the transmission is completed.
* The middleware sends the ``WIFI_MW_EVENT_TERMINATED`` event to the application, and the scan sequence is over.

.. _Wi-Fi scan results payload format:

Scan results payload format
+++++++++++++++++++++++++++

As the middleware automatically sends the scan results for location solving, it has control over the format used for the uplink.

There are 2 formats possible, that the user can choose depending on the solver used:

* `WIFI_MW_PAYLOAD_MAC`: contains only the MAC addresses of the detected Access Points
* `WIFI_MW_PAYLOAD_MAC_RSSI`: contains the MAC addresses of the detected Access Points and the strength of the signal at which it has been detected.


.. _table-wifi-payload-mac:

.. table:: Wi-Fi results payload format with MAC addresses only.

    +-----------------+-----------------+-----+-----------------+
    | AP1 MAC address | AP2 MAC address | ... | APn MAC address |
    +=================+=================+=====+=================+
    | 6 bytes         | 6 bytes         | ... | 6 bytes         |
    +-----------------+-----------------+-----+-----------------+


.. _table-wifi-payload-mac-rssi:

.. table:: Wi-Fi results payload format with MAC addresses and RSSI.

    +----------+-----------------+----------+-----------------+-----+----------+-----------------+
    | AP1 RSSI | AP1 MAC address | AP2 RSSI | AP2 MAC address | ... | APn RSSI | APn MAC address |
    +==========+=================+==========+=================+=====+==========+=================+
    | 1 byte   | 6 bytes         | 1 byte   | 6 bytes         | ... | 1 byte   | 6 bytes         |
    +----------+-----------------+----------+-----------------+-----+----------+-----------------+


The maximum size of the complete payload has been kept under 51 bytes to match with the maximum payload size allowed by the LoRaWAN Regional Parameters for most regions (there are few exceptions like DR0 of the US915 region which therefore cannot be used).

.. _LoRaWAN datarate considerations for Wi-Fi:

LoRaWAN datarate considerations
+++++++++++++++++++++++++++++++

As seen in the section `Wi-Fi scan results payload format`_ , due to the maximum length of the scan results payload, some LoRaWAN datarates cannot be used to transmit the results.

Also, depending on the region of operation and how often it is required to get a position for the final application, much care should be taken of the datarates used.

It is **mandatory** to disable the "Network Controlled" mode for Adaptative Datarate (ADR) and rather used custom profiles.
In this custom profiles, it is generally more efficient to use fast datarates, and increase the number of retransmission.

It is to be noted that the same ADR configuration will be used for sending geolocation scan results and application specific payloads.

.. _Cancelling a Wi-Fi scan:

Cancelling a Wi-Fi scan
+++++++++++++++++++++++

The middleware API provides a function ``wifi_mw_scan_cancel()`` which can be used by the user application to cancel a programmed scan & send operation.

It is important to note that a scan can be cancelled only if the corresponding task has not yet been launched. A scan task which has been launched cannot be aborted and will complete (both scan and send).

A scan task is considered "launched" when the delay to start the scan has elapsed and the Radio Planner has granted access to the radio.


.. _Wi-Fi API:

API
+++

Refer to the ``wifi/src/wifi_middleware.h`` file.
