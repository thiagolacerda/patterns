#ifndef GPSPOINT_H
#define GPSPOINT_H

#include <cstdint>
#include "utils.h"

class GPSPoint {
public:
    GPSPoint();

    GPSPoint(double latitude, double longitude, uint32_t timestamp, uint32_t trajectoryId);

    double latitude() const { return m_latitude; }

    double longitude() const { return m_longitude; }

    uint32_t timestamp() const { return m_timestamp; }

    uint32_t trajectoryId() const { return m_trajectoryId; }

    double distanceToPoint(const GPSPoint&, Utils::DistanceType type = Utils::FlatSpherical);

    bool operator<(GPSPoint* point) const;

    void dump();

private:
    double m_latitude;
    double m_longitude;
    uint32_t m_timestamp;
    uint32_t m_trajectoryId;
};

#endif  // GPSPOINT_H
