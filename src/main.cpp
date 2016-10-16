#include "mbed.h"
#include "Simple-LoRaWAN.h"
#include "Gps.h"
#include "settings.h"
#include "Packet.h"

using namespace SimpleLoRaWAN;

Serial pc(USBTX, USBRX);
Serial gps_serial(p28,p27);
Gps* gps;
Ticker gps_ticker;
Ticker send_ticker;

DigitalOut fix_led(LED4);
DigitalOut tx_led(LED1);

Node* node;

void debug_tx_buffer()
{
    // for(int i = 0; i<sizeof(txBuffer); i++)
    // {
    //     pc.printf("0x%02x ", txBuffer[i]);
    // }
    // pc.printf("\r\n");
}

void show_gps_info()
{
   gps->debug(&pc);
}

void send_gps_info()
{
    if(gps->fix){
        pc.printf("Sending packet:");
        uint8_t* packet = Packet::build(gps);
        //debug_tx_buffer();
        node->send(reinterpret_cast<char*>(packet), 9);
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
        fix_led = gps->fix;
    }
}
