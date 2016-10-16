#pragma once

#include "Gps.h"

class Packet
{
public:
    static uint8_t* build(Gps* gps);
};
