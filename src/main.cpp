/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2015 Semtech

Description: MBED LoRaWAN example application

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
#include "mbed.h"

DigitalOut txled(LED1);
DigitalOut rxled(LED2);

#include "lmic.h"
#include "config.h"

static uint8_t mydata[] = "Hello, world!";

// LEDs and Frame jobs
osjob_t rxLedJob;
osjob_t txLedJob;
osjob_t sendFrameJob;

//////////////////////////////////////////////////
// Utility functions
//////////////////////////////////////////////////
/*!
 * \brief Computes a random number between min and max
 *
 * \param [IN] min range minimum value
 * \param [IN] max range maximum value
 * \retval random random value in range min..max
 */
int32_t randr( int32_t min, int32_t max )
{
    return ( int32_t )rand( ) % ( max - min + 1 ) + min;
}

//////////////////////////////////////////////////
// MAIN - INITIALIZATION AND STARTUP
//////////////////////////////////////////////////

static void onRxLed( osjob_t* j )
{
    rxled = 0;
}

static void onTxLed( osjob_t* j )
{
    txled = 0;
}

void processRxFrame( void )
{

}

static void onSendFrame( osjob_t* j )
{
    LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0);

    // Blink Tx LED
    txled = 1;
    os_setTimedCallback( &txLedJob, os_getTime( ) + ms2osticks( 25 ), onTxLed );
}

// Initialization job
static void onInit( osjob_t* j )
{
    // reset MAC state
    LMIC_reset( );
    LMIC_setAdrMode( LORAWAN_ADR_ON );
#if defined(CFG_eu868)
    LMIC_setDrTxpow( DR_SF8, 14 );
#elif defined(CFG_us915)
    LMIC_setDrTxpow( DR_SF8, 14 );
#endif

    LMIC_setSession( LORAWAN_NET_ID, LORAWAN_DEV_ADDR, NwkSKey, ArtSKey );
    onSendFrame( NULL );

    // init done - onEvent( ) callback will be invoked...
}

int main( void )
{
    osjob_t initjob;

    // initialize runtime env
    os_init( );
    // setup initial job
    os_setCallback( &initjob, onInit );
    // execute scheduled jobs and events
    os_runloop( );
    // (not reached)
}

//////////////////////////////////////////////////
// LMIC EVENT CALLBACK
//////////////////////////////////////////////////
void onEvent( ev_t ev )
{
    bool txOn = false;

    switch( ev )
    {
    // network joined, session established
    case EV_JOINED:
        txOn = true;
        break;
    // scheduled data sent (optionally data received)
    case EV_TXCOMPLETE:
        // Check if we have a downlink on either Rx1 or Rx2 windows
        if( ( LMIC.txrxFlags & ( TXRX_DNW1 | TXRX_DNW2 ) ) != 0 )
        {
            rxled = 1;
            os_setTimedCallback( &rxLedJob, os_getTime( ) + ms2osticks( 25 ), onRxLed );

            if( LMIC.dataLen != 0 )
            { // data received in rx slot after tx
                processRxFrame( );
            }
        }
        txOn = true;
        break;
    default:
        break;
    }
    if( txOn == true )
    {
        //Sends frame every APP_TX_DUTYCYCLE +/- APP_TX_DUTYCYCLE_RND random time (if not duty cycle limited)
        os_setTimedCallback( &sendFrameJob,
                             os_getTime( ) + ms2osticks( APP_TX_DUTYCYCLE + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND ) ),
                             onSendFrame );

        ////Sends frame as soon as possible (duty cylce limitations)
        //onSendFrame( NULL );
    }
}
