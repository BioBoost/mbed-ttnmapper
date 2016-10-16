#include "Logger.h"

Logger::Logger(Serial* serial)
{
    this->serial = serial;
}

void Logger::packet(uint8_t* packet)
{
    for(int i = 0; i<9; i++)
    {
        serial->printf("0x%02x ", packet[i]);
    }
    serial->printf("\r\n");
}

void Logger::gps(Gps* gps)
{
    gps->debug(serial);
}
