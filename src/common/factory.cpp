#include "factory.h"

#include "lifemapdbdecoder.h"
#include "trucksdbdecoder.h"
#include "truckspaperdbdecoder.h"

DatabaseDecoder* Factory::dbDecoderInstance(Config::DBDecoder decoder)
{
    if (decoder == Config::LifeMap)
        return new LifeMapDBDecoder(Config::decoderParameters());
    if (decoder == Config::Trucks)
        return new TrucksDBDecoder(Config::decoderParameters());
    if (decoder == Config::TrucksPaper)
        return new TrucksPaperDBDecoder(Config::decoderParameters());

    return nullptr;
}
