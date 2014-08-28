#ifndef GPSPoint_h
#define GPSPoint_h

#include "utils.h"

class GPSPoint {
public:
    GPSPoint();

    GPSPoint(double latitude, double longitude, unsigned long timestamp, unsigned long trajectoryId);

    double latitude() const { return m_latitude; }
    void setLatitude(double latitude) { m_latitude = latitude; }

    double longitude() const { return m_longitude; }
    void setLongitude(double longitude) { m_longitude = longitude; }

    unsigned long timestamp() const { return m_timestamp; }
    void setTimestamp(unsigned long timestamp) { m_timestamp = timestamp; }

    unsigned long trajectoryId() const { return m_trajectoryId; }
    void setTrajectoryId(unsigned long trajectoryId) { m_trajectoryId = trajectoryId; }

    double distanceToPoint(const GPSPoint&, Utils::DistanceType type = Utils::FlatSpherical);

    bool operator<(GPSPoint*) const;

    void dump();

private:
    double m_latitude;
    double m_longitude;
    unsigned long m_timestamp;
    unsigned long m_trajectoryId;
};

#endif // GPSPoint_h
