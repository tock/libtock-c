/*!
 * @ingroup   apps_geolocation
 * @file      main_geolocation_gnss_wifi.h
 *
 * @brief     LoRa Basics Modem LR11XX Geolocation GNSS and Wi-Fi example.
 *
 * @copyright
 * @parblock
 * The Clear BSD License
 * Copyright Semtech Corporation 2021. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * @endparblock
 */

/*!
 * @addtogroup apps_geolocation
 * LoRa Basics Modem LR11XX Geolocation example
 * @{
 */

#ifndef MAIN_GEOLOCATION_GNSS_WIFI_H
#define MAIN_GEOLOCATION_GNSS_WIFI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*
 * -----------------------------------------------------------------------------
 * --- Application Configuration -----------------------------------------------
 */

/**
 * @brief Let the application autonomously set the assistance position for GNSS geolocation (default, autonomous mode)
 */
#define MODEM_EXAMPLE_ASSISTANCE_POSITION_AUTO_DEFAULT false

/**
 * @brief User defined assistance position for GNSS geolocation (not used by default, autonomous mode)
 */
// #define MODEM_EXAMPLE_ASSISTANCE_POSITION_LAT_DEFAULT ( 45.181454 )
// #define MODEM_EXAMPLE_ASSISTANCE_POSITION_LONG_DEFAULT ( 5.720893 )
// #define MODEM_EXAMPLE_ASSISTANCE_POSITION_TEXT_DEFAULT "Grenoble, FRANCE"
#define MODEM_EXAMPLE_ASSISTANCE_POSITION_LAT_DEFAULT ( 22.576814 )
#define MODEM_EXAMPLE_ASSISTANCE_POSITION_LONG_DEFAULT ( 113.922068 )
#define MODEM_EXAMPLE_ASSISTANCE_POSITION_TEXT_DEFAULT "Shenzhen, CHINA"

/*
 * -----------------------------------------------------------------------------
 * --- Geolocation scan group Configuration ------------------------------------
 */

/*!
 * @brief Defines the delay before starting the next scan sequence, value in [s].
 */
#define GEOLOCATION_SCAN_PERIOD_DEFAULT 60

/*!
 * @brief Defines the scan mode used for GNSS (STATIC or MOBILE).
 */
#define GNSS_SCAN_MODE_DEFAULT GNSS_MW_MODE_MOBILE

#ifndef MODEM_EXAMPLE_ASSISTANCE_POSITION_AUTO
#define MODEM_EXAMPLE_ASSISTANCE_POSITION_AUTO MODEM_EXAMPLE_ASSISTANCE_POSITION_AUTO_DEFAULT
#endif  // MODEM_EXAMPLE_ASSISTANCE_POSITION_AUTO

#ifndef MODEM_EXAMPLE_ASSISTANCE_POSITION_LAT
#define MODEM_EXAMPLE_ASSISTANCE_POSITION_LAT MODEM_EXAMPLE_ASSISTANCE_POSITION_LAT_DEFAULT
#endif  // MODEM_EXAMPLE_ASSISTANCE_POSITION_LAT

#ifndef MODEM_EXAMPLE_ASSISTANCE_POSITION_LONG
#define MODEM_EXAMPLE_ASSISTANCE_POSITION_LONG MODEM_EXAMPLE_ASSISTANCE_POSITION_LONG_DEFAULT
#endif  // MODEM_EXAMPLE_ASSISTANCE_POSITION_LONG

#ifndef MODEM_EXAMPLE_ASSISTANCE_POSITION_TEXT
#define MODEM_EXAMPLE_ASSISTANCE_POSITION_TEXT MODEM_EXAMPLE_ASSISTANCE_POSITION_TEXT_DEFAULT
#endif  // MODEM_EXAMPLE_ASSISTANCE_POSITION_TEXT

#ifndef GEOLOCATION_SCAN_PERIOD
#define GEOLOCATION_SCAN_PERIOD GEOLOCATION_SCAN_PERIOD_DEFAULT
#endif  // GEOLOCATION_SCAN_PERIOD

#ifndef GNSS_SCAN_MODE
#define GNSS_SCAN_MODE GNSS_SCAN_MODE_DEFAULT
#endif  // GNSS_SCAN_MODE

/*
 * -----------------------------------------------------------------------------
 * --- LoRaWAN Configuration ---------------------------------------------------
 */

/*!
 * @brief ADR custom list and retransmission parameters for EU868 / IN865 / RU864 / AU915 / CN470 /AS923 / KR920 regions
 */
#define CUSTOM_NB_TRANS_DR5_DR3 2
#define ADR_CUSTOM_LIST_DR5_DR3                        \
    {                                                  \
        5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 3, 3 \
    } /* 125kHz - SF7, SF8, SF9 */

/*!
 * @brief ADR custom list and retransmission parameters for US915 region
 */
#define CUSTOM_NB_TRANS_US915 2
#define ADR_CUSTOM_LIST_US915                          \
    {                                                  \
        3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1 \
    } /* 125kHz - SF7, SF8, SF9 */

/*!
 * @brief ADR custom list and retransmission parameters for WW2G4 region
 */
#define CUSTOM_NB_TRANS_WW2G4 2
#define ADR_CUSTOM_LIST_WW2G4                          \
    {                                                  \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 \
    } /* SF12 */

#ifdef __cplusplus
}
#endif

#endif  // MAIN_GEOLOCATION_GNSS_WIFI_H

/*!
 * @}
 */

/* --- EOF ------------------------------------------------------------------ */
