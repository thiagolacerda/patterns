#include "truckspaperdecoder.h"

#include <memory>
#include "componentfactory.h"
#include "datamodel.h"
#include "gpspoint.h"
#include "rawdata.h"

void TrucksPaperDecoder::decodeData(const RawData& data)
{
    auto longitude = data.getAsDouble(1);
    auto latitude = data.getAsDouble(2);
    auto point = std::make_shared<GPSPoint>(latitude, longitude, latitude, longitude, data.getAsUInt64_t(3), data.getAsUInt32_t(0));
    DataModel model(point);

    sendDataToListeners(model);
}

REGISTER_DATA_DECODER("truckspaperdecoder", TrucksPaperDecoder::instance);

