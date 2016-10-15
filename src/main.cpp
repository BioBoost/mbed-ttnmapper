#include "mbed.h"
#include "Simple-LoRaWAN.h"
#include "stdio.h"
#include "Gps.h"

using namespace SimpleLoRaWAN;
uint8_t appEui[8]  = { 0x3A, 0x04, 0x00, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 }; // MSBF
uint8_t devEui[8]  = { 0x77, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 }; // MSBF

uint32_t devAddr    = 0xECB5C5E5;
uint8_t nwksKey[16] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C }; // LSBF
uint8_t appKey[16]  = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C }; // LSBF

Serial pc(USBTX, USBRX);
Serial gps_serial(p28,p27);
Gps* gps;
Ticker gps_ticker;

void show_gps_info()
{
   //check if enough time has passed to warrant printing GPS info to screen
   //note if refresh_Time is too low or pc.baud is too low, GPS data may be lost during printing
   gps->debug(&pc);
}

Node* node;

int main(void)
{
    pc.baud(115200);
    node = new ABP::Node(devAddr, nwksKey, appKey);
    //node = new OTAA::Node(appEui, devEui, appKey);

    gps = new Gps(&gps_serial);
    gps_ticker.attach(&show_gps_info, 2.0);

    wait(1.0);
    printf("send message\r\n");
    node->send("Hello from Simple-LoRaWAN", 25);

    while(true){
        node->process();
        gps->run();
    }
}
