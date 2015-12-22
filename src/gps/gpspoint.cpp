#include "gpspoint.h"

#include <time.h>
#include "config.h"
#include "trajectory.h"

GPSPoint::GPSPoint()
    : m_latitude(0)
    , m_latitudeMeters(0)
    , m_longitude(0)
    , m_longitudeMeters(0)
    , m_timestamp(0)
    , m_trajectoryId(0)
{ }

GPSPoint::GPSPoint(double latitude, double longitude, uint64_t timestamp, uint32_t trajectoryId)
    : m_latitude(latitude)
    , m_latitudeMeters(0)
    , m_longitude(longitude)
    , m_longitudeMeters(0)
    , m_timestamp(timestamp)
    , m_trajectoryId(trajectoryId)
{
    if (Config::coordinateSystem() == Config::Metric) {
        m_latitudeMeters = m_latitude;
        m_longitudeMeters = m_longitude;
    } else {
        Utils::latLongToMeters(m_latitude, m_longitude, &m_latitudeMeters, &m_longitudeMeters);
    }
}

double GPSPoint::distanceToPoint(const GPSPoint& other)
{
    return Utils::distance(*this, other);
}

bool GPSPoint::operator<(const GPSPoint& other) const
{
    return m_timestamp <= other.timestamp();
}

bool GPSPoint::operator==(const GPSPoint& other) const
{
    return m_timestamp == other.timestamp() &&
        m_trajectoryId == other.trajectoryId() &&
        Utils::fuzzyEqual(m_latitude, other.latitude()) &&
        Utils::fuzzyEqual(m_longitude, other.longitude()) &&
        // we accept up to 5 meters of inaccuracy when converting from lat long to meters
        Utils::fuzzyEqual(m_latitudeMeters, other.latitudeMeters(), 5.0) &&
        Utils::fuzzyEqual(m_longitudeMeters, other.longitudeMeters(), 5.0);
}

void GPSPoint::dump()
{
    std::cout << this << std::endl;
}

std::ostream& operator<<(std::ostream& stream, const GPSPoint& point)
{
    char buf[32];
    ctime_r((const time_t*) &point.m_timestamp, buf);
    stream << "Point -> trajectory: " << point.m_trajectoryId << ", lat long: (" << point.m_latitude << ", "
        << point.m_longitude << "), lat long in meters: (" << point.m_latitudeMeters << ", " << point.m_longitudeMeters
        << "), time: " << point.m_timestamp << ", date: " << buf;
    return stream;
}
