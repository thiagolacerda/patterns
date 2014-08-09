#include "gpspoint.h"

#include <iostream>
#include "trajectory.h"

GPSPoint::GPSPoint()
    : m_latitude(0)
    , m_longitude(0)
    , m_timestamp(0)
    , m_trajectory(nullptr)
{ }

GPSPoint::GPSPoint(double latitude, double longitude, unsigned long timestamp, Trajectory* trajectory)
    : m_latitude(latitude)
    , m_longitude(longitude)
    , m_timestamp(timestamp)
    , m_trajectory(trajectory)
{ }

double GPSPoint::distanceToPoint(const GPSPoint& point, Utils::DistanceType type)
{
    return Utils::distance(*this, point, type);
}

bool GPSPoint::operator<(GPSPoint* other) const
{
    return m_timestamp < other->timestamp();
}

void GPSPoint::dump()
{
    std::cout << "Point: tID: " << m_trajectory->id() << ", lat long: ("
        << m_latitude << ", " << m_longitude << "), " << m_timestamp << std::endl;
}
