#include "config.h"

#include "patterns.h"

Config::DBDecoder Config::m_decoderInUse = Config::LifeMap;
std::vector<std::string> Config::m_decoderParameters;
unsigned Config::m_trajectoriesPerFlock = 0;
double Config::m_gridSize = 0;
unsigned Config::m_timeSlotSize = 0;
Config::CoordinateSystem Config::m_coordinateSystem = Config::Cartesian;

void Config::setDecoder(char* decoderName)
{
    std::string decoder(decoderName);
    if (decoder.compare("trucks") == 0)
        Config::m_decoderInUse = Config::Trucks;
}