/*
   RadioLib Non-Arduino Tock Library LoRaWAN connection details

   Licensed under the MIT or Apache License

   Copyright (c) 2024 Alistair Francis <alistair@alistair23.me>
 */

/* These need to be updated to use values from your LoRaWAN server */
uint64_t joinEUI = 0x0000000000000000;
uint64_t devEUI  = 0x0000000000000000;
uint8_t appKey[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t nwkKey[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// regional choices: EU868, US915, AU915, AS923, IN865, KR920, CN780, CN500
const LoRaWANBand_t* Region = &AU915;
const uint8_t subBand       = 2;
