#pragma once

#include "mbed.h"
#include "lmic.h"

DigitalOut txled(LED1);
DigitalOut rxled(LED2);


static uint8_t mydata[] = "Hello, world!";

// LEDs and Frame jobs
osjob_t rxLedJob;
osjob_t txLedJob;
osjob_t sendFrameJob;


int32_t randr( int32_t min, int32_t max );
void onTxLed( osjob_t* j );
void onSendFrame( osjob_t* j );
void onInit( osjob_t* j );
int init( void );
int loop(void);
void onEvent( ev_t ev );
