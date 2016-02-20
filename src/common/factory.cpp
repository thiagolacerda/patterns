#include "factory.h"

#include "berlinmoddecoder.h"
#include "lifemapdbdecoder.h"
#include "sortedfiledecoder.h"
#include "tdrivedecoder.h"
#include "trucksdbdecoder.h"
#include "truckspaperdbdecoder.h"

DatabaseDecoder* Factory::dbDecoderInstance(Config::DBDecoder decoder)
{
    if (decoder == Config::BerlinMOD)
        return new BerlinMODDecoder(Config::decoderParameters());
    if (decoder == Config::LifeMap)
        return new LifeMapDBDecoder(Config::decoderParameters());
    if (decoder == Config::SortedFile)
        return new SortedFileDecoder(Config::decoderParameters());
    if (decoder == Config::TDrive)
        return new TDriveDecoder(Config::decoderParameters());
    if (decoder == Config::Trucks)
        return new TrucksDBDecoder(Config::decoderParameters());
    if (decoder == Config::TrucksPaper)
        return new TrucksPaperDBDecoder(Config::decoderParameters());

    return nullptr;
}
