#include "gpspointbuffererlistener.h"

#include "componentfactory.h"
#include "datamodel.h"
#include "gpspoint.h"
#include "gpspointbuffererlistenerdata.h"

GPSPointBuffererListener::GPSPointBuffererListener(const std::unordered_map<std::string, std::string>& parameters)
    : DataListener(parameters)
    , m_timeSlotSize(1)
    , m_patternLength(1)
    , m_outlierSpeedCutoff(0)
    , m_interpolate(false)
    , m_lastTimeSlotSeen(0)
{
    auto iter = m_parameters.find("timeSlotSize");
    if (iter != m_parameters.end())
        m_timeSlotSize = stoul(iter->second);

    iter = m_parameters.find("patternLength");
    if (iter != m_parameters.end())
        m_patternLength = stoul(iter->second);

    iter = m_parameters.find("outlierSpeedCutoff");
    if (iter != m_parameters.end())
        m_outlierSpeedCutoff = stod(iter->second);
}

void GPSPointBuffererListener::onDataReceived(const DataModel& dataModel)
{
    const auto& point = dataModel.get<GPSPoint>();
    const uint64_t timestamp = point->timestamp();
    const uint32_t trajectoryId = point->trajectoryId();
    const double latMeters = point->latitudeMeters();
    const double longMeters = point->longitudeMeters();
    if (isOutlier(timestamp, trajectoryId, latMeters, longMeters))
        return;

    uint64_t index = timestamp / m_timeSlotSize;
    if (index > m_lastTimeSlotSeen) {
        if (index - m_lastTimeSlotSeen > 1) {
            complete();
        } else {
            if (m_pointsPerTimeSlot.size() >= m_patternLength) {
                const auto& firstMapPair = m_pointsPerTimeSlot.begin();
                GPSPointBuffererListenerData data(firstMapPair->second, m_pointPresence, m_pointsPerTimeSlot.size(), firstMapPair->first);
                sendDataToProcessors(data);
                m_pointsPerTimeSlot.erase(firstMapPair->first);
                shiftPresenceMap();
            }
        }
        m_lastTimeSlotSeen = index;
    }
    insertInMap(point, index);
    if (m_interpolate || m_outlierSpeedCutoff > 0)
        m_lastTimestampPerTrajectory[trajectoryId] = timestamp;
}

void GPSPointBuffererListener::insertInMap(const std::shared_ptr<GPSPoint>& point, uint64_t timeSlot)
{
    uint32_t trajectoryId = point->trajectoryId();
    m_pointsPerTimeSlot[timeSlot][trajectoryId].push_back(point);
    addPresence(trajectoryId, m_pointsPerTimeSlot.size() - 1);

    if (!m_interpolate || timeSlot < 2)
        return;

    timeSlot -= 2;
    const auto& lastTimestampIter = m_lastTimestampPerTrajectory.find(trajectoryId);
    if (lastTimestampIter == m_lastTimestampPerTrajectory.end() ||
        (lastTimestampIter->second / m_timeSlotSize) != timeSlot)
        return;

    const auto& points2TimesBeforeIter = m_pointsPerTimeSlot.find(timeSlot);
    if (points2TimesBeforeIter == m_pointsPerTimeSlot.end())
        return;

    const auto& base = points2TimesBeforeIter->second[trajectoryId].back();
    const auto& interpolated = Utils::interpolate(*point, *base);

    ++timeSlot;
    m_pointsPerTimeSlot[timeSlot][trajectoryId].push_back(interpolated);
    addPresence(trajectoryId, m_pointsPerTimeSlot.size() - 2);
}

bool GPSPointBuffererListener::isOutlier(uint64_t timestamp, uint32_t trajectoryId, double latMeters, double longMeters)
{
    if (m_outlierSpeedCutoff == 0)
        return false;

    const auto& lastTimestampIter = m_lastTimestampPerTrajectory.find(trajectoryId);
    if (lastTimestampIter == m_lastTimestampPerTrajectory.end())
        return false;

    uint64_t index = lastTimestampIter->second / m_timeSlotSize;
    const auto& pointsMapIter = m_pointsPerTimeSlot.find(index);
    if (pointsMapIter == m_pointsPerTimeSlot.end())
        return false;

    const auto& lastPoint = pointsMapIter->second[trajectoryId].back();
    const uint64_t latestTimestamp = lastPoint->timestamp();

    double distance = Utils::distance(longMeters, latMeters, lastPoint->longitudeMeters(), lastPoint->latitudeMeters());
    double timeDiff = timestamp == latestTimestamp ? 1 : (timestamp - latestTimestamp);
    if ((distance / timeDiff) >= m_outlierSpeedCutoff)
        return true;

    return false;
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
    for (auto iter = m_pointsPerTimeSlot.begin(); iter != m_pointsPerTimeSlot.end(); ) {
        GPSPointBuffererListenerData data(iter->second, m_pointPresence, m_pointsPerTimeSlot.size(), iter->first);
        sendDataToProcessors(data);
        iter = m_pointsPerTimeSlot.erase(iter);
        shiftPresenceMap();
    }
    m_pointPresence.clear();
    m_pointsPerTimeSlot.clear();
    m_lastTimestampPerTrajectory.clear();
}

void GPSPointBuffererListener::onDataProviderFinished()
{
    complete();
    endDataHandler();
}

REGISTER_DATA_LISTENER("gpspointbuffererlistener", "g", GPSPointBuffererListener::instance);

