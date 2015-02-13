#include "gpspointtemporalprocessor.h"

#include <algorithm>
#include <iostream>
#include <math.h>
#include "config.h"
#include "gpspoint.h"
#include "utils.h"

void GPSPointTemporalProcessor::processGPSTuple(const std::tuple<unsigned long, double, double, unsigned long>& tuple)
{
    unsigned long tID;
    double latitude;
    double longitude;
    unsigned long timestamp;
    std::tie(tID, latitude, longitude, timestamp) = tuple;

    if (Config::coordinateSystem() == Config::Cartesian)
        Utils::latLongToMeters(latitude, longitude, &latitude, &longitude);

    unsigned index = Config::timeSlotSize() > 0 ? timestamp / Config::timeSlotSize() : timestamp;
    std::shared_ptr<GPSPoint> point(new GPSPoint(latitude, longitude, timestamp, tID));
    if (Config::automaticTimeSlot()) {
        auto iter = m_lastTimestampPerTrajectory.find(tID);
        if (iter != m_lastTimestampPerTrajectory.end()) {
            if (iter->second > timestamp)
                std::cout << "ERROR!!!!! tID: " << tID << " " << iter->second << " > " << timestamp << std::endl;

            unsigned long diff = timestamp - iter->second;
            m_timeDiffs.push_back(diff);
            m_timeDiffsSum += diff;
        }

        m_lastTimestampPerTrajectory[tID] = timestamp;
        m_points.push_back(point);
    } else {
        if (m_pointsPerTimeSlot.find(index) == m_pointsPerTimeSlot.end())
            m_pointsPerTimeSlot[index] = std::vector<std::shared_ptr<GPSPoint>>();

        m_pointsPerTimeSlot[index].push_back(point);
    }
}

void GPSPointTemporalProcessor::removeOutliers() {
    bool stop = false;
    unsigned long n = m_timeDiffs.size();
    while (n > 0 && !stop) {
        long double mean = (long double)(m_timeDiffsSum) / n;
        long double accumulator = 0;
        std::for_each(m_timeDiffs.begin(), m_timeDiffs.end(), [&accumulator, mean](unsigned long n) {
            accumulator += pow(mean - n, 2);
        });
        long double stdDev = sqrt(accumulator / n);
        long double reject = (1.96 * (n - 1)) / (sqrt(n) * sqrt(n - 2 + 1.96 * 1.96));
        std::vector<unsigned long>::iterator rejected = m_timeDiffs.end();
        for (auto iter = m_timeDiffs.begin(); iter != m_timeDiffs.end(); ++iter) {
            long double value = fabsl((*iter - mean) / stdDev);
            if (value > reject && (rejected == m_timeDiffs.end() || (*rejected) < value))
                rejected = iter;
        }
        if (rejected != m_timeDiffs.end()) {
            m_timeDiffsSum -= *rejected;
            m_timeDiffs.erase(rejected);
        } else
            stop = true;

        n = m_timeDiffs.size();
    }
}

void GPSPointTemporalProcessor::postProcessPoints()
{
    if (!Config::automaticTimeSlot())
        return;

    removeOutliers();
    unsigned long slot = m_timeDiffsSum / m_timeDiffs.size();
    Config::setTimeSlotSize(slot);
    m_timeDiffs.clear();

    for (const std::shared_ptr<GPSPoint>& point : m_points) {
        unsigned index = Config::timeSlotSize() > 0 ? point->timestamp() / Config::timeSlotSize() : point->timestamp();
        if (m_pointsPerTimeSlot.find(index) == m_pointsPerTimeSlot.end())
            m_pointsPerTimeSlot[index] = std::vector<std::shared_ptr<GPSPoint>>();

        m_pointsPerTimeSlot[index].push_back(point);
    }
    m_points.clear();
}

void GPSPointTemporalProcessor::dumpPointsMap()
{
    std::cout << "Number of time slots: " << m_pointsPerTimeSlot.size() << std::endl;
    for (auto iter = m_pointsPerTimeSlot.begin(); iter != m_pointsPerTimeSlot.end(); ++iter) {
        std::cout << "\t***time slot: " << iter->first << ", number of points: " << iter->second.size() << std::endl;
        const std::vector<std::shared_ptr<GPSPoint>>& points = iter->second;
        for (const std::shared_ptr<GPSPoint>& point : points) {
            std::cout << "\t\t";
            point->dump();
        }
    }
}
