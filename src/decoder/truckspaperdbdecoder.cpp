#include "truckspaperdbdecoder.h"

#include <string.h>
#include "filedbmanager.h"
#include "gpstuplelistener.h"

TrucksPaperDBDecoder::TrucksPaperDBDecoder(const std::vector<std::string>& parameters)
    : DatabaseDecoder(new FileDBManager('\t'))
    , m_path(parameters.at(0))
{
}

void TrucksPaperDBDecoder::doDecodeRow(void* row)
{
    uint32_t tId = m_manager->getColumnAsUInt32(row, 0);
    double longitude = m_manager->getColumnAsDouble(row, 1);
    double latitude = m_manager->getColumnAsDouble(row, 2);
    uint64_t timestamp = m_manager->getColumnAsUInt64(row, 3);

    m_listener->processGPSTuple(std::make_tuple(tId, latitude, longitude, timestamp));
}

uint64_t TrucksPaperDBDecoder::retrievePoints()
{
    m_manager->connect(m_path, "", "");
    uint64_t retrieved = m_manager->retrievePoints();
    m_manager->disconnect();
    return retrieved;
}
