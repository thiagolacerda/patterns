#include "brinkhoffmntgdecoder.h"

void BrinkhoffMNTGDecoder::doDecodeRow(void* row)
{
    uint32_t tId = m_manager->getColumnAsUInt32(row, 0);
    uint64_t timestamp = m_manager->getColumnAsUInt64(row, 1);
    double longitude = m_manager->getColumnAsDouble(row, 3);
    double latitude = m_manager->getColumnAsDouble(row, 4);

    m_listener(std::make_tuple(tId, latitude, longitude, timestamp));
}
