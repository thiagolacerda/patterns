#ifndef GPSPointTemporalProcessor_h
#define GPSPointTemporalProcessor_h

#include <memory>
#include <map>
#include <vector>
#include <unordered_map>
#include "gpstuplelistener.h"

class GPSPoint;

class GPSPointTemporalProcessor : public GPSTupleListener {
public:
    GPSPointTemporalProcessor()
        : m_timeDiffsSum(0)
    {}

    void processGPSTuple(const std::tuple<unsigned long, double, double, unsigned long>&) override;
    void postProcessPoints();
    std::map<unsigned, std::vector<std::shared_ptr<GPSPoint>>> pointsPerTimeSlot() const { return m_pointsPerTimeSlot; }
    void releasePoints() { m_pointsPerTimeSlot.clear(); }
    void dumpPointsMap();

private:
    void removeOutliers();
    std::map<unsigned, std::vector<std::shared_ptr<GPSPoint>>> m_pointsPerTimeSlot;
    std::vector<std::shared_ptr<GPSPoint>> m_points;
    std::unordered_map<unsigned long, unsigned long> m_lastTimestampPerTrajectory;
    std::vector<unsigned long> m_timeDiffs;
    unsigned long long m_timeDiffsSum;
};

#endif // GPSPointTemporalProcessor_h
