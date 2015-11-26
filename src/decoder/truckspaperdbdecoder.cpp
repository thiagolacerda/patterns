#include "truckspaperdbdecoder.h"

#include "gpstuplelistener.h"

void TrucksPaperDBDecoder::doDecodeRow(void* row)
{
    uint32_t tId = m_manager->getColumnAsUInt32(row, 0);
    double longitude = m_manager->getColumnAsDouble(row, 1);
    double latitude = m_manager->getColumnAsDouble(row, 2);
    uint64_t timestamp = m_manager->getColumnAsUInt64(row, 3);

    m_listener->processGPSTuple(std::make_tuple(tId, latitude, longitude, timestamp));
}

