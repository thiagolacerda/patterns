#include "berlinmoddecoder.h"

#include <cstring>
#include <stdlib.h>
#include "componentfactory.h"
#include "datamodel.h"
#include "gpspoint.h"
#include "rawdata.h"

void BerlinMODDecoder::decodeData(const RawData& data)
{
    std::string dateAndHourStr = data.getAsString(3);
    unsigned len = dateAndHourStr.length();
    if (len < 16)
        return;

    double longitude = data.getAsDouble(6);
    double latitude = data.getAsDouble(7);

    double latMeters = 0;
    double longMeters = 0;
    Utils::latLongToMeters(latitude, longitude, latMeters, longMeters);

    // tm.tm_year = year - 1900
    struct tm dateTime;
    memset(&dateTime, 0, sizeof(struct tm));
    dateTime.tm_year = atoi(dateAndHourStr.substr(0, 4).c_str()) - 1900;
    dateTime.tm_mon = atoi(dateAndHourStr.substr(5, 2).c_str()) - 1;
    dateTime.tm_mday = atoi(dateAndHourStr.substr(8, 2).c_str());
    dateTime.tm_hour = atoi(dateAndHourStr.substr(11, 2).c_str());
    dateTime.tm_min = atoi(dateAndHourStr.substr(14, 2).c_str());
    dateTime.tm_sec = len < 19 ? 0 : atoi(dateAndHourStr.substr(17, 2).c_str());

    auto point = std::make_shared<GPSPoint>(latitude, longitude, latMeters, longMeters, mktime(&dateTime),
        data.getAsUInt32_t(0));
    DataModel model(point);

    sendDataToListeners(model);
}

REGISTER_DATA_DECODER("berlinmoddecoder", "b", BerlinMODDecoder::instance);

