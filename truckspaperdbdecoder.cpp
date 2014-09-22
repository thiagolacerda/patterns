#include "truckspaperdbdecoder.h"

#include <string.h>
#include "filedbmanager.h"
#include "gpstuplelistener.h"

TrucksPaperDBDecoder::TrucksPaperDBDecoder(const std::vector<std::string>& parameters)
    : DatabaseDecoder(new FileDBManager('\t'))
    , m_path(parameters.at(0))
{
    m_manager->setDecoder(this);
}

void TrucksPaperDBDecoder::decodeRow(void* row)
{
    unsigned long tId = m_manager->getColumnAsUnsignedLong(row, 0);
    double latitude = m_manager->getColumnAsDouble(row, 1);
    double longitude = m_manager->getColumnAsDouble(row, 2);
    unsigned long timestamp = m_manager->getColumnAsUnsignedLong(row, 3);

    m_listener->processGPSTuple(std::make_tuple(tId, latitude, longitude, timestamp));
}

void TrucksPaperDBDecoder::retrievePoints()
{
    m_manager->connect(m_path, "", "");
    m_manager->retrievePoints();
    m_manager->disconnect();
}