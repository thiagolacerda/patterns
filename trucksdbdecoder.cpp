#include "trucksdbdecoder.h"

#include <string.h>
#include "filedbmanager.h"
#include "gpstuplelistener.h"

TrucksDBDecoder::TrucksDBDecoder(const std::vector<std::string>& parameters)
    : DatabaseDecoder(new FileDBManager(';'))
    , m_path(parameters.at(0))
{
}

void TrucksDBDecoder::doDecodeRow(void* row)
{
    unsigned long tId;
    double latitude;
    double longitude;
    std::string dateStr;
    std::string hourStr;
    unsigned long timestamp;
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    tId = m_manager->getColumnAsUnsignedLong(row, 0);
    dateStr = m_manager->getColumnAsString(row, 2);
    hourStr = m_manager->getColumnAsString(row, 3);
    latitude = m_manager->getColumnAsDouble(row, 4);
    longitude = m_manager->getColumnAsDouble(row, 5);

    // tm.tm_year = year - 1900
    tm.tm_year = atoi(dateStr.substr(6, 4).c_str()) - 1900;
    tm.tm_mon = atoi(dateStr.substr(4, 2).c_str());
    tm.tm_mday = atoi(dateStr.substr(0, 2).c_str());
    tm.tm_hour = atoi(hourStr.substr(0, 2).c_str());
    tm.tm_min = atoi(hourStr.substr(3, 2).c_str());
    tm.tm_sec = atoi(hourStr.substr(6, 2).c_str());
    timestamp = mktime(&tm);

    m_listener->processGPSTuple(std::make_tuple(tId, latitude, longitude, timestamp));
}

unsigned long long TrucksDBDecoder::retrievePoints()
{
    m_manager->connect(m_path, "", "");
    unsigned long long retrieved = m_manager->retrievePoints();
    m_manager->disconnect();
    return retrieved;
}
