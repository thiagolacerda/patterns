#include "gpspointbuffererlistener.h"

#include "componentfactory.h"
#include "datamodel.h"
#include "gpspoint.h"
#include "gpspointlistenerdata.h"

void GPSPointBuffererListener::onPointAdded(uint32_t id, uint64_t timeSlot)
{
    uint64_t offset = timeSlot - m_pointsPerTimeSlot.begin()->first;
    addPresence(id, offset);
}

void GPSPointBuffererListener::onTimeSlotDone(uint64_t newTimeSlot)
{
    if (newTimeSlot - m_lastTimeSlotSeen > 1) {
        complete();
    } else {
        if (m_pointsPerTimeSlot.size() >= m_patternLength) {
            const auto& firstMapPair = m_pointsPerTimeSlot.begin();
            GPSPointListenerData data(firstMapPair->second, firstMapPair->first);
            data.m_extra.reset(new GPSPointListenerExtraData(m_pointPresence, m_pointsPerTimeSlot.size()));
            sendDataToProcessors(data);
            m_pointsPerTimeSlot.erase(firstMapPair->first);
        }
    }
}

void GPSPointBuffererListener::sendDataToProcessors(const ProcessorData& data)
{
    GPSPointListener::sendDataToProcessors(data);
    shiftPresenceMap();
}

void GPSPointBuffererListener::addPresence(uint32_t tID, uint64_t n)
{
    m_pointPresence[tID] |= (1 << n);
}

void GPSPointBuffererListener::shiftPresenceMap()
{
    for (auto iter = m_pointPresence.begin(); iter != m_pointPresence.end();) {
        if (iter->second) {
            iter->second >>= 1;
            ++iter;
        } else {
            iter = m_pointPresence.erase(iter);
        }
    }
}

void GPSPointBuffererListener::complete()
{
    GPSPointListener::complete();
    m_pointPresence.clear();
}

REGISTER_DATA_LISTENER("gpspointbuffererlistener", "g", GPSPointBuffererListener::instance);

