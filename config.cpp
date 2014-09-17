#include "config.h"

#include "patterns.h"

Config::DBDecoder Config::m_decoderInUse = Config::LifeMap;
std::vector<std::string> Config::m_decoderParameters;
unsigned Config::m_trajectoriesPerFlock = 0;
double Config::m_gridSize = 0;
double Config::m_radiusSquared = 0;
double Config::m_radius = 0;
unsigned Config::m_timeSlotSize = 0;
Config::CoordinateSystem Config::m_coordinateSystem = Config::Cartesian;

void Config::setDecoder(char* decoderName)
{
    std::string decoder(decoderName);
    if (decoder.compare("trucks") == 0)
        Config::m_decoderInUse = Config::Trucks;
}
void Config::setGridSize(double size)
{
    m_gridSize = size;
    m_radius = m_gridSize / 2.0;
    m_radiusSquared = m_radius * m_radius;
}
