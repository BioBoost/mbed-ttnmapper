#include "mbed.h"
#include "Simple-LoRaWAN.h"
#include "stdio.h"
#include "Gps.h"
#include "settings.h"

using namespace SimpleLoRaWAN;


Serial pc(USBTX, USBRX);
Serial gps_serial(p28,p27);
Gps* gps;
Ticker gps_ticker;
Ticker send_ticker;

Node* node;

uint8_t txBuffer[9];

void build_packet(Gps* gps)
{
    uint32_t LatitudeBinary, LongitudeBinary;
    uint16_t altitudeGps;
    uint8_t hdopGps;

    LatitudeBinary = ((gps->latitude_in_degrees() + 90) / 180.0) * 16777215;
    LongitudeBinary = ((gps->longitude_in_degrees() + 180) / 360.0) * 16777215;

    txBuffer[0] = ( LatitudeBinary >> 16 ) & 0xFF;
    txBuffer[1] = ( LatitudeBinary >> 8 ) & 0xFF;
    txBuffer[2] = LatitudeBinary & 0xFF;

    txBuffer[3] = ( LongitudeBinary >> 16 ) & 0xFF;
    txBuffer[4] = ( LongitudeBinary >> 8 ) & 0xFF;
    txBuffer[5] = LongitudeBinary & 0xFF;

    altitudeGps = (int) gps->altitude;
    txBuffer[6] = ( altitudeGps >> 8 ) & 0xFF;
    txBuffer[7] = altitudeGps & 0xFF;

    hdopGps = (int)gps->HDOP*10.0;
    txBuffer[8] = hdopGps & 0xFF;

    /*
    Time: 14:52:13.0
    Date: 15/10/2016
    Fix: 1
    Quality: 2
    Location: 51.173592N, 3.217686E
    HDOP:  0.87
    Speed:  0.00 knots
    Angle: 302.56
    Altitude:  6.80
    Satellites: 10
    {0xc8, 0xc7, 0xbb, 0x82, 0x49, 0xc2, 0x00, 0x06, 0x00}
    */

}

void debug_tx_buffer()
{
    for(int i = 0; i<sizeof(txBuffer); i++)
    {
        pc.printf("0x%02x ", txBuffer[i]);
    }
    pc.printf("\r\n");
}

void show_gps_info()
{
   gps->debug(&pc);
}

void send_gps_info()
{
    if(gps->fix){
        pc.printf("Sending packet:");
        build_packet(gps);
        debug_tx_buffer();
        node->send(reinterpret_cast<char*>(txBuffer), 9);
    }
}


int main(void)
{
    pc.baud(115200);
    node = new ABP::Node(devAddr, nwksKey, appKey);
    //node = new OTAA::Node(appEui, devEui, appKey);

    gps = new Gps(&gps_serial);
    gps_ticker.attach(&show_gps_info, 2.0);
    send_ticker.attach(&send_gps_info, 10.0);

    wait(1.0);

    while(true){
        node->process();
        gps->run();
    }
}
