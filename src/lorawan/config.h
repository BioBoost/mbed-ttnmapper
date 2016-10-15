#pragma once

/*!
 * Defines the network ID when using personalization activation procedure
 */
#define LORAWAN_NET_ID                              ( uint32_t )0x00000000

/*!
 * Defines the device address when using personalization activation procedure
 */
#define LORAWAN_DEV_ADDR                            ( uint32_t )0x0D4991A1

/*!
 * Defines the application data transmission duty cycle
 */
#define APP_TX_DUTYCYCLE                            5000 // 5 [s] value in ms
#define APP_TX_DUTYCYCLE_RND                        1000 // 1 [s] value in ms

/*!
 * LoRaWAN Adaptative Data Rate
 */
#define LORAWAN_ADR_ON                              1

/*!
 * LoRaWAN application port
 */
#define LORAWAN_APP_PORT                            15

//////////////////////////////////////////////////
// CONFIGURATION (FOR APPLICATION CALLBACKS BELOW)
//////////////////////////////////////////////////

// application router ID (LSBF)
static const uint8_t AppEui[8] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// unique device ID (LSBF)
static const u1_t DevEui[8] =
{
    0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF
};

// device-specific AES key (derived from device EUI)
static const uint8_t DevKey[16] =
{
    0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
    0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};

// network session key
static uint8_t NwkSKey[] =
{
    0xAC, 0xF7, 0xFD, 0xEA, 0xEE, 0x33, 0xF8, 0xD0,
    0xA6, 0x80, 0x4A, 0xD0, 0x25, 0xC6, 0x41, 0x7B
};

// application session key
static uint8_t ArtSKey[] =
{
    0x29, 0x77, 0x31, 0x0A, 0x2B, 0x92, 0x35, 0xE1,
    0x72, 0x96, 0x91, 0x39, 0xCC, 0xEB, 0x70, 0x66
};

//////////////////////////////////////////////////
// APPLICATION CALLBACKS
//////////////////////////////////////////////////

// provide application router ID (8 bytes, LSBF)
void os_getArtEui( uint8_t *buf )
{
    memcpy( buf, AppEui, 8 );
}

// provide device ID (8 bytes, LSBF)
void os_getDevEui( uint8_t *buf )
{
    memcpy( buf, DevEui, 8 );
}

// provide device key (16 bytes)
void os_getDevKey( uint8_t *buf )
{
    memcpy( buf, DevKey, 16 );
}
