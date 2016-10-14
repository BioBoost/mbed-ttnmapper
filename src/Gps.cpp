#include "Gps.h"
#include "mbed.h"

Gps::Gps(Serial* serial) : Adafruit_GPS(serial)
{
    //gps = new Adafruit_GPS(serial); //object of Adafruit's GPS class

    begin(9600);  //sets baud rate for GPS communication; note this may be changed via Adafruit_GPS::sendCommand(char *)
                        //a list of GPS commands is available at http://www.adafruit.com/datasheets/PMTK_A08.pdf

    sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //these commands are defined in MBed_Adafruit_GPS.h; a link is provided there for command creation
    sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    sendCommand(PGCMD_ANTENNA);
}

Gps::~Gps()
{

}

void Gps::debug(Serial* serial)
{
    serial->printf("Time: %d:%d:%d.%u\n", hour, minute, seconds, milliseconds);
    serial->printf("Date: %d/%d/20%d\n", day, month, year);
    serial->printf("Fix: %d\n", (int) fix);
    serial->printf("Quality: %d\n", (int) fixquality);
    if (fix) {
        serial->printf("Location: %5.2f%c, %5.2f%c\n", latitude, lat, longitude, lon);
        serial->printf("Speed: %5.2f knots\n", speed);
        serial->printf("Angle: %5.2f\n", angle);
        serial->printf("Altitude: %5.2f\n", altitude);
        serial->printf("Satellites: %d\n", satellites);
    }
}

void Gps::run()
{
    read();   //queries the GPS

    //check if we recieved a new message from GPS, if so, attempt to parse it,
    if ( newNMEAreceived() ) {
        parse(lastNMEA());
//        if ( !parse(lastNMEA()) ) {
//            continue;
//        }
    }
}
