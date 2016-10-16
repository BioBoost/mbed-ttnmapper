#include "Packet.h"

using namespace std;

uint8_t* Packet::build(Gps* gps)
{
    static uint8_t txBuffer[9];

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
    return txBuffer;
}
