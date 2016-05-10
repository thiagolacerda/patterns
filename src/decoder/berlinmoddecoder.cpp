#include "berlinmoddecoder.h"

#include <cstring>
#include <stdlib.h>
#include <string.h>

void BerlinMODDecoder::doDecodeRow(void* row)
{
    std::string dateAndHourStr = m_manager->getColumnAsString(row, 3);
    unsigned len = dateAndHourStr.length();
    if (len < 16)
        return;

    uint32_t tId = m_manager->getColumnAsUInt32(row, 0);
    double longitude = m_manager->getColumnAsDouble(row, 6);
    double latitude = m_manager->getColumnAsDouble(row, 7);

    // tm.tm_year = year - 1900
    struct tm dateTime;
    memset(&dateTime, 0, sizeof(struct tm));
    dateTime.tm_year = atoi(dateAndHourStr.substr(0, 4).c_str()) - 1900;
    dateTime.tm_mon = atoi(dateAndHourStr.substr(5, 2).c_str()) - 1;
    dateTime.tm_mday = atoi(dateAndHourStr.substr(8, 2).c_str());
    dateTime.tm_hour = atoi(dateAndHourStr.substr(11, 2).c_str());
    dateTime.tm_min = atoi(dateAndHourStr.substr(14, 2).c_str());
    dateTime.tm_sec = len < 19 ? 0 : atoi(dateAndHourStr.substr(17, 2).c_str());
    uint64_t timestamp = mktime(&dateTime);

    m_listener(std::make_tuple(tId, latitude, longitude, timestamp));
}
