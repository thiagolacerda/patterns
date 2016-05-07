#pragma once

#include <map>
#include <vector>
#include "datalistener.h"

class GPSPoint;

class GPSPointBuffererListener : public DataListener {
public:
    GPSPointBuffererListener(const std::unordered_map<std::string, std::string>& parameters);

    void onDataReceived(const DataModel& dataModel) override;

    void onDataProviderFinished() override;

    static GPSPointBuffererListener* instance(const std::unordered_map<std::string, std::string>& parameters)
    {
        return new GPSPointBuffererListener(parameters);
    }

private:
    void addPresence(uint32_t tID, uint64_t n);
    void complete();
    void insertInMap(const std::shared_ptr<GPSPoint>& dataModel, uint64_t timeSlot);
    bool isOutlier(uint64_t timestamp, uint32_t trajectoryId, double latMeters, double longMeters);
    void shiftPresenceMap();

    uint32_t m_timeSlotSize;
    uint32_t m_patternLength;
    double m_outlierSpeedCutoff;
    bool m_interpolate;
    uint64_t m_lastTimeSlotSeen;
    std::map<uint64_t, std::unordered_map<uint32_t, std::vector<std::shared_ptr<GPSPoint>>>> m_pointsPerTimeSlot;
    std::unordered_map<uint32_t, uint64_t> m_pointPresence;
    std::unordered_map<uint32_t, uint64_t> m_lastTimestampPerTrajectory;
};

