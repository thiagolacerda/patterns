#include "customdecoder.h"

#include <memory>
#include "datamodel.h"
#include "gpspoint.h"
#include "rawdata.h"

void CustomDecoder::decodeData(const RawData& data)
{
    auto latitude = data.getAsDouble(1);
    auto longitude = data.getAsDouble(2);
    double latMeters = 0;
    double longMeters = 0;
    Utils::latLongToMeters(latitude, longitude, latMeters, longMeters);
    auto point = std::make_shared<GPSPoint>(latitude, longitude, latMeters, longMeters, data.getAsUInt64_t(3), data.getAsUInt32_t(0));
    DataModel model(point);

    sendDataToListeners(model);
}

