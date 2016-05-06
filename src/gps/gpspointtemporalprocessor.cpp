#include "gpspointtemporalprocessor.h"

#include <iostream>
#include "config.h"
#include "gpspoint.h"
#include "utils.h"

std::map<uint64_t, std::unordered_map<uint32_t, std::vector<std::shared_ptr<GPSPoint>>>>
    GPSPointTemporalProcessor::pointsPerTimeSlot() const
{
    return m_pointsPerTimeSlot;
}

void GPSPointTemporalProcessor::processGPSTuple(const std::tuple<uint32_t, double, double, uint64_t>& gpsTuple)
{
    uint32_t tID;
    double latitude;
    double longitude;
    uint64_t timestamp;
    std::tie(tID, latitude, longitude, timestamp) = gpsTuple;

    std::shared_ptr<GPSPoint> point(new GPSPoint(latitude, longitude, timestamp, tID));
    if (isOutlier(point))
        return;

    uint64_t index = timestamp / Config::timeSlotSize();
    if (Config::onlineProcessing()) {
        if (index > m_lastTimeSlotSeen) {
            const auto& firstMapPair = m_pointsPerTimeSlot.begin();
            m_listener(firstMapPair->second, firstMapPair->first);
            m_pointsPerTimeSlot.clear();
            m_lastTimeSlotSeen = index;
        }
    }
    insertPointInMap(point, index, Config::interpolate());
    m_lastTimestampPerTrajectory[tID] = timestamp;
}

void GPSPointTemporalProcessor::complete()
{
    for (auto iter = m_pointsPerTimeSlot.begin(); iter != m_pointsPerTimeSlot.end(); ) {
        m_listener(iter->second, iter->first);
        iter = m_pointsPerTimeSlot.erase(iter);
    }
    m_pointsPerTimeSlot.clear();
    m_lastTimestampPerTrajectory.clear();
}

bool GPSPointTemporalProcessor::isOutlier(const std::shared_ptr<GPSPoint>& point)
{
    if (Config::outlierSpeedCutOff() == -1)
        return false;

    const auto& lastTimestampIter = m_lastTimestampPerTrajectory.find(point->trajectoryId());
    if (lastTimestampIter == m_lastTimestampPerTrajectory.end())
        return false;

    uint64_t index = lastTimestampIter->second / Config::timeSlotSize();
    const auto& pointsMapIter = m_pointsPerTimeSlot.find(index);
    if (pointsMapIter == m_pointsPerTimeSlot.end())
        return false;

    const std::shared_ptr<GPSPoint>& latest = pointsMapIter->second[point->trajectoryId()].back();
    double distance = point->distanceToPoint(*latest);
    double timeDiff = point->timestamp() == latest->timestamp() ? 1 : (point->timestamp() - latest->timestamp());
    if (distance / timeDiff >= Config::outlierSpeedCutOff())
        return true;

    return false;
}

void GPSPointTemporalProcessor::insertPointInMap(const std::shared_ptr<GPSPoint>& point, uint64_t timeSlot,
    bool tryInterpolate)
{
    uint32_t trajectoryId = point->trajectoryId();
    m_pointsPerTimeSlot[timeSlot][trajectoryId].push_back(point);

    if (!tryInterpolate || timeSlot < 2)
        return;

    timeSlot -= 2;
    const auto& lastTimestampIter = m_lastTimestampPerTrajectory.find(trajectoryId);
    if (lastTimestampIter == m_lastTimestampPerTrajectory.end() ||
        (lastTimestampIter->second / Config::timeSlotSize()) != timeSlot)
        return;

    const auto& points2TimesBeforeIter = m_pointsPerTimeSlot.find(timeSlot);
    if (points2TimesBeforeIter == m_pointsPerTimeSlot.end())
        return;

    const std::shared_ptr<GPSPoint>& basePoint = points2TimesBeforeIter->second[trajectoryId].back();
    std::shared_ptr<GPSPoint> interpolated = Utils::interpolate(*point, *basePoint);

    ++timeSlot;
    m_pointsPerTimeSlot[timeSlot][trajectoryId].push_back(interpolated);
}

void GPSPointTemporalProcessor::dumpPointsMap()
{
    std::cout << "Number of time slots: " << m_pointsPerTimeSlot.size() << std::endl;
    for (auto iter = m_pointsPerTimeSlot.begin(); iter != m_pointsPerTimeSlot.end(); ++iter) {
        std::cout << "\t***time slot: " << iter->first << ", number unique of points: " << iter->second.size() << std::endl;
        const std::unordered_map<uint32_t, std::vector<std::shared_ptr<GPSPoint>>>& pointsMap = iter->second;
        for (const auto& mapPair : pointsMap) {
            const auto& points = mapPair.second;
            for (const std::shared_ptr<GPSPoint>& point : points) {
                std::cout << "\t\t";
                point->dump();
            }
        }
    }
}
