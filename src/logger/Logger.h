#pragma once

#include "mbed.h"
#include "Packet.h"
#include "Gps.h"

class Logger
{
public:
    Logger(Serial* serial);
    void packet(uint8_t* packet);
    void gps(Gps* gps);
private:
    Serial* serial;
};
