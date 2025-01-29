#include "lr1_stack_mac_layer.h"
#include "lr1mac_utilities.h"
#include "lorawan_api.h"

#include "us_915_ttn.h"

void region_us_915_the_things_network_init(void)
{
    // Get a pointer to the mac layer data structure.
    lr1_stack_mac_t* lr1_mac = lorawan_api_stack_mac_get();

    uint8_t* channel_index_enabled = lr1_mac->real->region.us915.channel_index_enabled;


    // TTN only supports bank 2. Disable all first.
    // Tx 125 kHz channels.
    for( uint8_t i = 0; i < NUMBER_OF_TX_CHANNEL_US_915 - 8; i++ )
    {
        SMTC_PUT_BIT8( channel_index_enabled, i, CHANNEL_DISABLED );
    }
    // Tx 500 kHz channels
    for( uint8_t i = NUMBER_OF_TX_CHANNEL_US_915 - 8; i < NUMBER_OF_TX_CHANNEL_US_915; i++ )
    {
        SMTC_PUT_BIT8( channel_index_enabled, i, CHANNEL_DISABLED );
    }

    // Now enable bank 2
    for( uint8_t i = 8; i < 15; i++ )
    {
        SMTC_PUT_BIT8( channel_index_enabled, i, CHANNEL_ENABLED );
    }
}