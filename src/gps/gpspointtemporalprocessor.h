#ifndef GPSPOINTTEMPORALPROCESSOR_H
#define GPSPOINTTEMPORALPROCESSOR_H

#include <map>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>

class GPSPoint;

using ListenerFunc = std::function<void (const std::unordered_map<uint32_t, std::vector<std::shared_ptr<GPSPoint>>>&,
    uint64_t)>;

class GPSPointTemporalProcessor {
public:
    GPSPointTemporalProcessor()
        : m_lastTimeSlotSeen(0)
    {}

    void processGPSTuple(const std::tuple<uint32_t, double, double, uint64_t>&);
    std::map<uint64_t, std::unordered_map<uint32_t, std::vector<std::shared_ptr<GPSPoint>>>> pointsPerTimeSlot() const;
    void releasePoints() { m_pointsPerTimeSlot.clear(); }
    void dumpPointsMap();
    void registerCallback(const ListenerFunc& func) { m_listener = func; }
    void complete();

private:
    void insertPointInMap(const std::shared_ptr<GPSPoint>& point, uint64_t timeSlot, bool tryInterpolate);
    bool isOutlier(const std::shared_ptr<GPSPoint>& point);

    ListenerFunc m_listener;
    std::map<uint64_t, std::unordered_map<uint32_t, std::vector<std::shared_ptr<GPSPoint>>>> m_pointsPerTimeSlot;
    std::unordered_map<uint32_t, uint64_t> m_lastTimestampPerTrajectory;
    uint64_t m_lastTimeSlotSeen;
};

#endif  // GPSPOINTTEMPORALPROCESSOR_H
