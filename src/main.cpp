#include "mbed.h"
#include "Gps.h"

Serial pc (USBTX, USBRX);
Serial * gps_Serial;
Gps* gps;
Ticker gps_ticker;

void show_gps_info()
{
   //check if enough time has passed to warrant printing GPS info to screen
   //note if refresh_Time is too low or pc.baud is too low, GPS data may be lost during printing
   gps->debug(&pc);
}

int main() {

   pc.baud(115200); //sets virtual COM serial communication to high rate; this is to allow more time to be spent on GPS retrieval
   gps_Serial = new Serial(p9,p10); //serial object for use w/ GPS
   gps = new Gps(gps_Serial);

   pc.printf("Connection established at 115200 baud...\n");

   wait(1);

   gps_ticker.attach(&show_gps_info, 2.0);

   while(true){
       gps->run();
   }
}
