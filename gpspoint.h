#ifndef GPSPoint_h
#define GPSPoint_h

#include "utils.h"

class GPSPoint {
public:
    GPSPoint()
        : m_latitude(0)
        , m_longitude(0)
        , m_timestamp(0)
    { }

    GPSPoint(double latitude, double longitude, double timestamp)
        : m_latitude(latitude)
        , m_longitude(longitude)
        , m_timestamp(timestamp)
    { }

    double latitude() const { return m_latitude; }
    void setLatitude(double latitude) { m_latitude = latitude; }

    double longitude() const { return m_longitude; }
    void setLongitude(double longitude) { m_longitude = longitude; }

    double timestamp() const { return m_timestamp; }
    void setTimestamp(double timestamp) { m_timestamp = timestamp; }

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
    double m_timestamp;
};

#endif // GPSPoint_h
