#include "disk.h"

#if !defined(NEWDESIGN)
#include <iostream>
#include "config.h"
#endif
#include "gpspoint.h"
#include "utils.h"

Disk::Disk()
#if defined(NEWDESIGN)
    : Disk(0, 0, 0, 0)
#else
    : Disk(0, 0, 0)
#endif
{
}

#if defined(NEWDESIGN)
Disk::Disk(double radius, double centerX, double centerY, uint64_t timestamp)
#else
Disk::Disk(double centerX, double centerY, uint64_t timestamp)
#endif
    : m_centerX(centerX)
    , m_centerLongitude(0)
    , m_centerY(centerY)
    , m_centerLatitude(0)
#if defined(NEWDESIGN)
    , m_radius(radius)
#else
    , m_radius(Config::radius())
#endif
    , m_timestamp(timestamp)
{
#if !defined(NEWDESIGN)
    if (Config::coordinateSystem() == Config::Metric) {
        m_centerLongitude = m_centerX;
        m_centerLatitude = m_centerY;
    } else {
        Utils::metersToLatLong(m_centerY, m_centerX, m_centerLatitude, m_centerLongitude);
    }
#endif
}

bool Disk::isPointInDisk(const std::shared_ptr<GPSPoint>& point)
{
    return isPointInDisk(point->longitudeMeters(), point->latitudeMeters());
}

bool Disk::isPointInDisk(double x, double y) const
{
    return Utils::fuzzyLessEqual(Utils::distance(m_centerX, m_centerY, x, y), m_radius);
}

void Disk::addPoint(const std::shared_ptr<GPSPoint>& point)
{
    m_points[point->trajectoryId()] = point;
}

uint32_t Disk::countIntersection(Disk* other) const
{
    auto begin1 = m_points.begin();
    auto begin2 = other->m_points.begin();
    auto end1 = m_points.end();
    auto end2 = other->m_points.end();
    --end1;
    --end2;
    if ((begin1->first < begin2->first && end1->first < begin2->first) ||
        (begin2->first < begin1->first && end2->first < begin1->first))
        return 0;

    uint32_t count = 0;
    while (begin1 != m_points.end() && begin2 != other->m_points.end()) {
        if (begin1->first < begin2->first) {
            ++begin1;
        } else if (begin2->first < begin1->first) {
            ++begin2;
        } else {
            ++count;
            ++begin1;
            ++begin2;
        }
    }

    return count;
}

#if !defined(NEWDESIGN)
void Disk::dumpPoints() const
{
    if (m_points.empty())
        return;

    auto iter = m_points.begin();
    std::cout << iter->second->trajectoryId();
    ++iter;
    for (; iter != m_points.end(); ++iter)
        std::cout << ", " << iter->second->trajectoryId();
    std::cout << std::endl;
}

void Disk::dump() const
{
    std::cout << "Disk, x: " << m_centerX << ", y: " << m_centerY << " trajectories: "
        << m_points.size() << std::endl;
}
#endif
