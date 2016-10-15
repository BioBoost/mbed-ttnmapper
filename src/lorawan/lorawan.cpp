
#include "lorawan.h"
#include "config.h"

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

void onTxLed( osjob_t* j )
{
    txled = 0;
}

void onSendFrame( osjob_t* j )
{
    LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0);

    // Blink Tx LED
    txled = 1;
    os_setTimedCallback( &txLedJob, os_getTime( ) + ms2osticks( 25 ), onTxLed );
}

// Initialization job
void onInit( osjob_t* j )
{
    // reset MAC state
    LMIC_reset( );
    LMIC_setAdrMode( LORAWAN_ADR_ON );
    LMIC_setDrTxpow( DR_SF7, 14 );

    LMIC_setSession( LORAWAN_NET_ID, LORAWAN_DEV_ADDR, NwkSKey, ArtSKey );
    onSendFrame( NULL );
}

int init( void )
{
    osjob_t initjob;

    // initialize runtime env
    os_init( );
    // setup initial job
    os_setCallback( &initjob, onInit );
    // execute scheduled jobs and events
}

int loop(void)
{
    while (true) {
        os_runloop( );
    }
}

//////////////////////////////////////////////////
// LMIC EVENT CALLBACK
//////////////////////////////////////////////////
void onEvent( ev_t ev )
{
    bool txOn = false;

    switch( ev )
    {
    // scheduled data sent (optionally data received)
    case EV_TXCOMPLETE:
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

        // //Sends frame as soon as possible (duty cylce limitations)
        // onSendFrame( NULL );
    }
}
