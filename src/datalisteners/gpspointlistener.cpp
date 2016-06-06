#include "gpspointlistener.h"

#include "componentfactory.h"
#include "datamodel.h"
#include "gpspoint.h"
#include "gpspointlistenerdata.h"

GPSPointListener::GPSPointListener(const std::unordered_map<std::string, std::string>& parameters)
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

void GPSPointListener::onDataReceived(const DataModel& dataModel)
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
        onTimeSlotDone(index);
        m_lastTimeSlotSeen = index;
    }

    insertInMap(point, index);
    if (m_interpolate || m_outlierSpeedCutoff > 0)
        m_lastTimestampPerTrajectory[trajectoryId] = timestamp;
}

void GPSPointListener::onTimeSlotDone(uint64_t)
{
    complete();
}

void GPSPointListener::insertInMap(const std::shared_ptr<GPSPoint>& point, uint64_t timeSlot)
{
    uint32_t trajectoryId = point->trajectoryId();
    m_pointsPerTimeSlot[timeSlot][trajectoryId].push_back(point);
    onPointAdded(trajectoryId, timeSlot);

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
    onPointAdded(trajectoryId, timeSlot);
}

bool GPSPointListener::isOutlier(uint64_t timestamp, uint32_t trajectoryId, double latMeters, double longMeters)
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

void GPSPointListener::complete()
{
    for (auto iter = m_pointsPerTimeSlot.begin(); iter != m_pointsPerTimeSlot.end(); ) {
        GPSPointListenerData data(iter->second, iter->first);
        sendDataToProcessors(data);
        iter = m_pointsPerTimeSlot.erase(iter);
    }
    m_pointsPerTimeSlot.clear();
    m_lastTimestampPerTrajectory.clear();
}

void GPSPointListener::onDataProviderFinished()
{
    complete();
    endDataHandler();
}

REGISTER_DATA_LISTENER("gpspointlistener", "p", GPSPointListener::instance);

