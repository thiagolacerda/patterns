#include "trucksdbdecoder.h"

#include <algorithm>
#include <fstream>
#include <iterator>
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

uint64_t TrucksDBDecoder::doRetrievePoints()
{
    m_manager->connect(m_path, "", "");
    uint64_t retrieved = m_manager->retrievePoints();
    m_manager->disconnect();
    return retrieved;
}

uint64_t TrucksDBDecoder::numberOfRecords()
{
    std::ifstream inFile(m_path);
    return std::count(std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>(), '\n');
}
