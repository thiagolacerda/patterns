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

    double flatDistanceSphericalToPoint(const GPSPoint& point) const
    {
        return Utils::flatDistanceSpherical(*this, point);
    }

    double flatDistanceEllipsoidalToPoint(const GPSPoint& point) const
    {
        return Utils::flatDistanceEllipsoidal(*this, point);
    }

    double tunnelDistanceToPoint(const GPSPoint& point) const
    {
        return Utils::tunnelDistance(*this, point);
    }

private:
    double m_latitude;
    double m_longitude;
};

#endif // GPSPoint_h
