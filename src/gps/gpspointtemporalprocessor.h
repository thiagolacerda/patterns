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
    uint64_t getMask(uint32_t trajectoryId) { return m_sequences[trajectoryId]; }
    uint64_t buffered() const { return m_pointsPerTimeSlot.size(); }

private:
    void insertPointInMap(const std::shared_ptr<GPSPoint>& point, uint64_t timeSlot, bool tryInterpolate);
    bool isOutlier(const std::shared_ptr<GPSPoint>& point);
    void addSequence(uint32_t tID, uint64_t n);
    void shiftSequences();

    ListenerFunc m_listener;
    std::map<uint64_t, std::unordered_map<uint32_t, std::vector<std::shared_ptr<GPSPoint>>>> m_pointsPerTimeSlot;
    std::unordered_map<uint32_t, uint64_t> m_lastTimestampPerTrajectory;
    uint64_t m_lastTimeSlotSeen;
    std::unordered_map<uint32_t, uint64_t> m_sequences;
};

#endif  // GPSPOINTTEMPORALPROCESSOR_H
