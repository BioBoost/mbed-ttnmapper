#pragma once

#include "mbed.h"
#include "MBed_Adafruit_GPS.h"

class Gps : public Adafruit_GPS
{
    public:
    Gps(Serial* serial);
    ~Gps();
    void debug(Serial* pc);
    void run();

    double latitude_in_degrees();
    double longitude_in_degrees();

    protected:
    static double convertDegMinToDecDeg (float degMin);
};
