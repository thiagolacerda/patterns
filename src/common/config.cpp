#include "config.h"

#include "patterns.h"

Config::DBDecoder Config::m_decoderInUse = Config::LifeMap;
std::vector<std::string> Config::m_decoderParameters;
uint32_t Config::m_flockLength = 0;
uint32_t Config::m_trajectoriesPerFlock = 0;
double Config::m_gridSize = 0;
double Config::m_radiusSquared = 0;
double Config::m_radius = 0;
uint32_t Config::m_timeSlotSize = 0;
Config::CoordinateSystem Config::m_coordinateSystem = Config::Cartesian;
bool Config::m_compatibilityMode = false;
bool Config::m_automaticTimeSlot = false;

void Config::setDecoder(char* decoderName)
{
    std::string decoder(decoderName);
    if (decoder.compare("trucks") == 0)
        Config::m_decoderInUse = Config::Trucks;
    else if (decoder.compare("truckspaper") == 0)
        Config::m_decoderInUse = Config::TrucksPaper;
}

void Config::setGridSize(double size)
{
    m_gridSize = size;
    m_radius = m_gridSize / 2.0;
    m_radiusSquared = m_radius * m_radius;
}

std::string Config::coordinateSystemName(CoordinateSystem coordSystem)
{
    switch (coordSystem) {
    case Config::WSG84:
        return "WSG84 GPS System";
    case Config::Cartesian:
        return "Cartesian System";
    case Config::CartesianNoConvert:
        return "Cartesian System but do not try to convert from GPS";
    default:
        return "Uknown type, falling to defalt: Cartesian";
    }
}
