#include "gpspoint.h"

#include <time.h>
#include <iostream>
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

GPSPoint::GPSPoint(double latitude, double longitude, uint32_t timestamp, uint32_t trajectoryId)
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

bool GPSPoint::operator<(GPSPoint* other) const
{
    return m_timestamp < other->timestamp();
}

void GPSPoint::dump()
{
    char buf[32];
    ctime_r((const time_t*) &m_timestamp, buf);
    std::cout << "Point: tID: " << m_trajectoryId << ", lat long: ("
        << m_latitude << ", " << m_longitude << "), " << m_timestamp
        << ", date: " << buf << std::endl;
}
