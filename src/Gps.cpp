#include "Gps.h"
#include "mbed.h"
//required for fmod()
#include <math.h>

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
        serial->printf("Location: %5.6f%c, %5.6f%c\n", latitude_in_degrees(), lat, longitude_in_degrees(), lon);
        serial->printf("HDOP: %5.2f\n", HDOP);
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
    }
}

// converts lat/long from Adafruit
// degree-minute format to decimal-degrees
double Gps::convertDegMinToDecDeg (float degMin) {
  double min = 0.0;
  double decDeg = 0.0;

  //get the minutes, fmod() requires double
  min = fmod((double)degMin, 100.0);

  //rebuild coordinates in decimal degrees
  degMin = (int) ( degMin / 100 );
  decDeg = degMin + ( min / 60 );

  return decDeg;
}

double Gps::latitude_in_degrees()
{
    return convertDegMinToDecDeg(latitude);
}

double Gps::longitude_in_degrees()
{
    return convertDegMinToDecDeg(longitude);
}
