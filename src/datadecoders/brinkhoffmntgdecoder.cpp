#include "brinkhoffmntgdecoder.h"

#include "componentfactory.h"
#include "datamodel.h"
#include "gpspoint.h"
#include "rawdata.h"

void BrinkhoffMNTGDecoder::decodeData(const RawData& data)
{
    double longitude = data.getAsDouble(3);
    double latitude = data.getAsDouble(4);

    double latMeters = 0;
    double longMeters = 0;
    Utils::latLongToMeters(latitude, longitude, latMeters, longMeters);

    auto point = std::make_shared<GPSPoint>(latitude, longitude, latMeters, longMeters, data.getAsUInt32_t(1),
        data.getAsUInt64_t(0));

    DataModel model(point);

    sendDataToListeners(model);
}

REGISTER_DATA_DECODER("brinkhoffdecoder", "h", BrinkhoffMNTGDecoder::instance);

