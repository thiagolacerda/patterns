#ifndef GPSPOINTTEMPORALPROCESSOR_H
#define GPSPOINTTEMPORALPROCESSOR_H

#include <cstdint>
#include <map>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>
#include "gpstuplelistener.h"

class GPSPoint;

class GPSPointTemporalProcessor : public GPSTupleListener {
public:
    GPSPointTemporalProcessor()
        : m_timeDiffsSum(0)
    {}

    void processGPSTuple(const std::tuple<uint32_t, double, double, uint64_t>&) override;
    void postProcessPoints();
    std::map<uint64_t, std::vector<std::shared_ptr<GPSPoint>>> pointsPerTimeSlot() const { return m_pointsPerTimeSlot; }
    void releasePoints() { m_pointsPerTimeSlot.clear(); }
    void dumpPointsMap();

private:
    void removeOutliers();
    std::map<uint64_t, std::vector<std::shared_ptr<GPSPoint>>> m_pointsPerTimeSlot;
    std::vector<std::shared_ptr<GPSPoint>> m_points;
    std::unordered_map<uint32_t, uint64_t> m_lastTimestampPerTrajectory;
    std::vector<uint64_t> m_timeDiffs;
    uint64_t m_timeDiffsSum;
};

#endif  // GPSPOINTTEMPORALPROCESSOR_H