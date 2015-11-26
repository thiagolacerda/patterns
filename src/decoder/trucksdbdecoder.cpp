#include "trucksdbdecoder.h"

#include <stdlib.h>
#include <string.h>
#include "gpstuplelistener.h"

void TrucksDBDecoder::doDecodeRow(void* row)
{
    uint32_t tId = m_manager->getColumnAsUInt32(row, 0);
    std::string dateStr = m_manager->getColumnAsString(row, 2);
    std::string hourStr = m_manager->getColumnAsString(row, 3);
    double longitude = m_manager->getColumnAsDouble(row, 4);
    double latitude = m_manager->getColumnAsDouble(row, 5);

    // tm.tm_year = year - 1900
    struct tm dateTime;
    memset(&dateTime, 0, sizeof(struct tm));
    dateTime.tm_year = atoi(dateStr.substr(6, 4).c_str()) - 1900;
    dateTime.tm_mon = atoi(dateStr.substr(4, 2).c_str()) - 1;
    dateTime.tm_mday = atoi(dateStr.substr(0, 2).c_str());
    dateTime.tm_hour = atoi(hourStr.substr(0, 2).c_str());
    dateTime.tm_min = atoi(hourStr.substr(3, 2).c_str());
    dateTime.tm_sec = atoi(hourStr.substr(6, 2).c_str());
    uint64_t timestamp = mktime(&dateTime);

    m_listener->processGPSTuple(std::make_tuple(tId, latitude, longitude, timestamp));
}

