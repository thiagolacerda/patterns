#include "tdrivedecoder.h"

#include <stdlib.h>
#include <string.h>

void TDriveDecoder::doDecodeRow(void* row)
{
    uint32_t tId = m_manager->getColumnAsUInt32(row, 0);
    std::string dateAndHourStr = m_manager->getColumnAsString(row, 1);
    double longitude = m_manager->getColumnAsDouble(row, 2);
    double latitude = m_manager->getColumnAsDouble(row, 3);

    // tm.tm_year = year - 1900
    struct tm dateTime;
    memset(&dateTime, 0, sizeof(struct tm));
    dateTime.tm_year = atoi(dateAndHourStr.substr(0, 4).c_str()) - 1900;
    dateTime.tm_mon = atoi(dateAndHourStr.substr(5, 2).c_str()) - 1;
    dateTime.tm_mday = atoi(dateAndHourStr.substr(8, 2).c_str());
    dateTime.tm_hour = atoi(dateAndHourStr.substr(11, 2).c_str());
    dateTime.tm_min = atoi(dateAndHourStr.substr(14, 2).c_str());
    dateTime.tm_sec = atoi(dateAndHourStr.substr(17, 2).c_str());
    uint64_t timestamp = mktime(&dateTime);

    m_listener(std::make_tuple(tId, latitude, longitude, timestamp));
}

