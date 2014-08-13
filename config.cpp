#include "config.h"

#include "patterns.h"

Config::DBDecoder Config::m_decoderInUse = Config::LifeMap;
std::vector<std::string> Config::m_decoderParameters;
double Config::m_gridSize = 0;
unsigned Config::m_timeSlotSize = 0;

void Config::setDecoder(char* decoderName)
{
    UNUSED(decoderName);
    // Do nothing for now, sine we only have one decoder
}