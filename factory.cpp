#include "factory.h"

#include "config.h"
#include "lifemapdbdecoder.h"
#include "trucksdbdecoder.h"
#include "truckspaperdbdecoder.h"

std::unique_ptr<DatabaseDecoder> Factory::m_dbDecoderInstance;
std::once_flag Factory::m_dbDecoderOnceFlag;

DatabaseDecoder* Factory::dbDecoderInstance()
{
    std::call_once(m_dbDecoderOnceFlag,
        [] {
            DatabaseDecoder* decoder = nullptr;
            if (Config::decoder() == Config::LifeMap)
                decoder = new LifeMapDBDecoder(Config::decoderParameters());
            else if (Config::decoder() == Config::Trucks)
                decoder = new TrucksDBDecoder(Config::decoderParameters());
            else if (Config::decoder() == Config::TrucksPaper)
                decoder = new TrucksPaperDBDecoder(Config::decoderParameters());
            m_dbDecoderInstance.reset(decoder);
    });
    return m_dbDecoderInstance.get();
}