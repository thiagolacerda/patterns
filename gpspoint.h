#ifndef GPSPoint_h
#define GPSPoint_h

#include "utils.h"

class Trajectory;

class GPSPoint {
public:
    GPSPoint()
        : m_latitude(0)
        , m_longitude(0)
        , m_timestamp(0)
        , m_trajectory(nullptr)
    { }

    GPSPoint(double latitude, double longitude, unsigned long timestamp, Trajectory* trajectory)
        : m_latitude(latitude)
        , m_longitude(longitude)
        , m_timestamp(timestamp)
        , m_trajectory(trajectory)
    { }

    double latitude() const { return m_latitude; }
    void setLatitude(double latitude) { m_latitude = latitude; }

    double longitude() const { return m_longitude; }
    void setLongitude(double longitude) { m_longitude = longitude; }

    unsigned long timestamp() const { return m_timestamp; }
    void setTimestamp(unsigned long timestamp) { m_timestamp = timestamp; }

    Trajectory* trajectory() const { return m_trajectory; }
    void setTrajectory(Trajectory* trajectory) { m_trajectory = trajectory; }

    double distanceToPoint(const GPSPoint& point, Utils::DistanceType type = Utils::FlatSpherical)
    {
        return Utils::distance(*this, point, type);
    }

    bool operator<(GPSPoint* other) const
    {
        return m_timestamp < other->timestamp();
    }

private:
    double m_latitude;
    double m_longitude;
    unsigned long m_timestamp;
    Trajectory* m_trajectory;
};

#endif // GPSPoint_h
