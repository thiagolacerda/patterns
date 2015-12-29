#include "gpspointtemporalprocessor.h"

#include <math.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "config.h"
#include "gpspoint.h"
#include "utils.h"

void GPSPointTemporalProcessor::processGPSTuple(const std::tuple<uint32_t, double, double, uint64_t>& gpsTuple)
{
    uint32_t tID;
    double latitude;
    double longitude;
    uint64_t timestamp;
    std::tie(tID, latitude, longitude, timestamp) = gpsTuple;

    std::shared_ptr<GPSPoint> point(new GPSPoint(latitude, longitude, timestamp, tID));
    if (Config::automaticTimeSlot()) {
        auto iter = m_lastTimestampPerTrajectory.find(tID);
        if (iter != m_lastTimestampPerTrajectory.end()) {
            if (iter->second > timestamp) {
                std::ostringstream oss;
                oss << "Data set is not ordered by time. Point " << point << " is older than the last one processed";
                throw std::runtime_error(oss.str());
            }

            uint64_t diff = timestamp - iter->second;
            m_timeDiffs.push_back(diff);
            m_timeDiffsSum += diff;
        }

        m_points.push_back(point);
    } else {
        uint64_t index = timestamp / Config::timeSlotSize();
        insertPointInMap(point, index, Config::interpolate());
    }
    m_lastTimestampPerTrajectory[tID] = timestamp;
}

void GPSPointTemporalProcessor::removeOutliers()
{
    bool stop = false;
    uint64_t n = m_timeDiffs.size();
    while (n > 0 && !stop) {
        long double mean = (long double)(m_timeDiffsSum) / n;
        long double accumulator = 0;
        std::for_each(m_timeDiffs.begin(), m_timeDiffs.end(), [&accumulator, mean](uint64_t n) {
            accumulator += pow(mean - n, 2);
        });
        long double stdDev = sqrt(accumulator / n);
        long double reject = (1.96 * (n - 1)) / (sqrt(n) * sqrt(n - 2 + 1.96 * 1.96));
        std::vector<uint64_t>::iterator rejected = m_timeDiffs.end();
        for (auto iter = m_timeDiffs.begin(); iter != m_timeDiffs.end(); ++iter) {
            long double value = fabsl((*iter - mean) / stdDev);
            if (value > reject && (rejected == m_timeDiffs.end() || (*rejected) < value))
                rejected = iter;
        }
        if (rejected != m_timeDiffs.end()) {
            m_timeDiffsSum -= *rejected;
            m_timeDiffs.erase(rejected);
        } else {
            stop = true;
        }

        n = m_timeDiffs.size();
    }
}

void GPSPointTemporalProcessor::postProcessPoints()
{
    if (!Config::automaticTimeSlot())
        return;

    removeOutliers();
    uint64_t slot = m_timeDiffsSum / m_timeDiffs.size();
    Config::setTimeSlotSize(slot);
    m_timeDiffs.clear();

    for (const std::shared_ptr<GPSPoint>& point : m_points) {
        uint64_t index = point->timestamp() / Config::timeSlotSize();

        insertPointInMap(point, index, Config::interpolate());
    }
    m_points.clear();
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
