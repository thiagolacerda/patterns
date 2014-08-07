#ifndef GPSPoint_h
#define GPSPoint_h

#include "utils.h"

class GPSPoint {
public:
    GPSPoint()
        : m_latitude(0)
        , m_longitude(0)
    { }

    GPSPoint(double latitude, double longitude)
        : m_latitude(latitude)
        , m_longitude(longitude)
    { }

    double latitude() const { return m_latitude; }
    void setLatitude(double latitude) { m_latitude = latitude; }

    double longitude() const { return m_longitude; }
    void setLongitude(double longitude) { m_longitude = longitude; }

    double distanceToPoint(const GPSPoint& point, Utils::DistanceType type = Utils::FlatSpherical)
    {
        return Utils::distance(*this, point, type);
    }
private:
    double m_latitude;
    double m_longitude;
};

#endif // GPSPoint_h
