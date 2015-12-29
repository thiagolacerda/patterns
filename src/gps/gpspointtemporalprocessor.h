#ifndef GPSPOINTTEMPORALPROCESSOR_H
#define GPSPOINTTEMPORALPROCESSOR_H

#include <cstdint>
#include <map>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>

class GPSPoint;

class GPSPointTemporalProcessor {
public:
    GPSPointTemporalProcessor()
        : m_timeDiffsSum(0)
    {}

    void processGPSTuple(const std::tuple<uint32_t, double, double, uint64_t>&);
    void postProcessPoints();
    std::map<uint64_t, std::unordered_map<uint32_t, std::vector<std::shared_ptr<GPSPoint>>>> pointsPerTimeSlot() const
    {
        return m_pointsPerTimeSlot;
    }
    void releasePoints() { m_pointsPerTimeSlot.clear(); }
    void dumpPointsMap();

private:
    void insertPointInMap(const std::shared_ptr<GPSPoint>& point, uint64_t timeSlot, bool tryInterpolate);
    void removeOutliers();
    std::map<uint64_t, std::unordered_map<uint32_t, std::vector<std::shared_ptr<GPSPoint>>>> m_pointsPerTimeSlot;
    std::vector<std::shared_ptr<GPSPoint>> m_points;
    std::unordered_map<uint32_t, uint64_t> m_lastTimestampPerTrajectory;
    std::vector<uint64_t> m_timeDiffs;
    uint64_t m_timeDiffsSum;
};

#endif  // GPSPOINTTEMPORALPROCESSOR_H
