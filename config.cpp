#include "config.h"

Config::DBDecoder Config::m_decoderInUse = Config::LifeMap;
std::vector<std::string> Config::m_decoderParameters;

void Config::setDecoder(char* decoderName)
{
    // Do nothing for now, sine we only have one decoder
}