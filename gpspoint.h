#ifndef GPSPoint_h
#define GPSPoint_h

#include "utils.h"

class Trajectory;

class GPSPoint {
public:
    GPSPoint();

    GPSPoint(double latitude, double longitude, unsigned long timestamp, Trajectory* trajectory);

    double latitude() const { return m_latitude; }
    void setLatitude(double latitude) { m_latitude = latitude; }

    double longitude() const { return m_longitude; }
    void setLongitude(double longitude) { m_longitude = longitude; }

    unsigned long timestamp() const { return m_timestamp; }
    void setTimestamp(unsigned long timestamp) { m_timestamp = timestamp; }

    Trajectory* trajectory() const { return m_trajectory; }
    void setTrajectory(Trajectory* trajectory) { m_trajectory = trajectory; }

    double distanceToPoint(const GPSPoint&, Utils::DistanceType type = Utils::FlatSpherical);

    bool operator<(GPSPoint*) const;


private:
    double m_latitude;
    double m_longitude;
    unsigned long m_timestamp;
    Trajectory* m_trajectory;
};

#endif // GPSPoint_h
