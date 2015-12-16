#include "sortedfiledecoder.h"

void SortedFileDecoder::doDecodeRow(void* row)
{
    uint32_t tId = m_manager->getColumnAsUInt32(row, 0);
    double latitude = m_manager->getColumnAsDouble(row, 1);
    double longitude = m_manager->getColumnAsDouble(row, 2);
    uint64_t timestamp = m_manager->getColumnAsUInt64(row, 3);

    m_listener(std::make_tuple(tId, latitude, longitude, timestamp));
}

