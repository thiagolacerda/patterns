#include "gpspointtemporalprocessor.h"

#include <iostream>
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
        TemporalInfo* pointInfo = nullptr;
        auto iter = m_trajectoriesTemporalInfo.find(tID);
        if (iter == m_trajectoriesTemporalInfo.end()) {
            pointInfo = new TemporalInfo(timestamp);
            m_trajectoriesTemporalInfo[tID] = pointInfo;
        } else {
            pointInfo = iter->second;
            pointInfo->addTime(timestamp);
        }
        if (m_pointsPerTimeSlotTemp.find(index) == m_pointsPerTimeSlotTemp.end())
            m_pointsPerTimeSlotTemp[index] = std::vector<std::shared_ptr<GPSPoint>>();

        m_pointsPerTimeSlotTemp[index].push_back(point);
    } else {
        if (m_pointsPerTimeSlot.find(index) == m_pointsPerTimeSlot.end())
            m_pointsPerTimeSlot[index] = std::vector<std::shared_ptr<GPSPoint>>();

        m_pointsPerTimeSlot[index].push_back(point);
    }
}

void GPSPointTemporalProcessor::postProcessPoints()
{
    if (!Config::automaticTimeSlot())
        return;

    double timeDiffAvgSum = 0;
    unsigned long numberOfTrajectories = m_trajectoriesTemporalInfo.size();
    for (auto iter = m_trajectoriesTemporalInfo.begin(); iter != m_trajectoriesTemporalInfo.end();) {
        timeDiffAvgSum += iter->second->average();
        iter = m_trajectoriesTemporalInfo.erase(iter);
    }

    unsigned long slot = timeDiffAvgSum / numberOfTrajectories;
    Config::setTimeSlotSize(slot);

    for (auto iter = m_pointsPerTimeSlotTemp.begin(); iter != m_pointsPerTimeSlotTemp.end();) {
        unsigned index = Config::timeSlotSize() > 0 ? iter->first / Config::timeSlotSize() : iter->first;
        if (m_pointsPerTimeSlot.find(index) == m_pointsPerTimeSlot.end())
            m_pointsPerTimeSlot[index] = iter->second;
        else
            m_pointsPerTimeSlot[index].insert(m_pointsPerTimeSlot[index].end(),
                iter->second.begin(), iter->second.end());

        iter = m_pointsPerTimeSlotTemp.erase(iter);
    }
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
