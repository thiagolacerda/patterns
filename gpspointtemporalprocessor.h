#ifndef GPSPointTemporalProcessor_h
#define GPSPointTemporalProcessor_h

#include <memory>
#include <map>
#include <vector>
#include <unordered_map>
#include "gpstuplelistener.h"

class GPSPoint;

class TemporalInfo {
public:
    TemporalInfo(unsigned long lastTimestamp)
        : m_diffs(0)
        , m_timeDiffSum(0)
        , m_lastTimestamp(lastTimestamp)
    {}

    double average() { return double(m_timeDiffSum) / double(m_diffs); }
    void addTime(unsigned t) {
        m_timeDiffSum += (t - m_lastTimestamp);
        m_lastTimestamp = t;
        ++m_diffs;
    }
private:
    unsigned long m_diffs;
    unsigned long long m_timeDiffSum;
    unsigned long m_lastTimestamp;
};

class GPSPointTemporalProcessor : public GPSTupleListener {
public:
    void processGPSTuple(const std::tuple<unsigned long, double, double, unsigned long>&) override;
    void postProcessPoints();
    std::map<unsigned, std::vector<std::shared_ptr<GPSPoint>>> pointsPerTimeSlot() const { return m_pointsPerTimeSlot; }
    void releasePoints() { m_pointsPerTimeSlot.clear(); }
    void dumpPointsMap();

private:
    std::map<unsigned, std::vector<std::shared_ptr<GPSPoint>>> m_pointsPerTimeSlot;
    std::unordered_map<unsigned, std::vector<std::shared_ptr<GPSPoint>>> m_pointsPerTimeSlotTemp;
    std::unordered_map<unsigned long, TemporalInfo*> m_trajectoriesTemporalInfo;
};

#endif // GPSPointTemporalProcessor_h
