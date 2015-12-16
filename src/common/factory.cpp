#include "factory.h"

#include "lifemapdbdecoder.h"
#include "sortedfiledecoder.h"
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
    if (decoder == Config::SortedFile)
        return new SortedFileDecoder(Config::decoderParameters());

    return nullptr;
}
