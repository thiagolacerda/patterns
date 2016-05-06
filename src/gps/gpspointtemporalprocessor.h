#ifndef GPSPOINTTEMPORALPROCESSOR_H
#define GPSPOINTTEMPORALPROCESSOR_H

#include <map>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>

class GPSPoint;

class GPSPointTemporalProcessor {
public:
    GPSPointTemporalProcessor()
    {}

    void processGPSTuple(const std::tuple<uint32_t, double, double, uint64_t>&);
    std::map<uint64_t, std::unordered_map<uint32_t, std::vector<std::shared_ptr<GPSPoint>>>> pointsPerTimeSlot() const;
    void releasePoints() { m_pointsPerTimeSlot.clear(); }
    void dumpPointsMap();

private:
    void insertPointInMap(const std::shared_ptr<GPSPoint>& point, uint64_t timeSlot, bool tryInterpolate);
    bool isOutlier(const std::shared_ptr<GPSPoint>& point);
    std::map<uint64_t, std::unordered_map<uint32_t, std::vector<std::shared_ptr<GPSPoint>>>> m_pointsPerTimeSlot;
    std::unordered_map<uint32_t, uint64_t> m_lastTimestampPerTrajectory;
};

#endif  // GPSPOINTTEMPORALPROCESSOR_H
