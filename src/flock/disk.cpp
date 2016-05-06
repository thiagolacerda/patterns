#include "disk.h"

#include <algorithm>
#include <iostream>
#include "config.h"
#include "gpspoint.h"
#include "grid.h"
#include "utils.h"

Disk::Disk()
    : Disk(0, 0, 0)
{
}

Disk::Disk(double centerX, double centerY, uint64_t timestamp)
    : m_centerX(centerX)
    , m_centerLongitude(0)
    , m_centerY(centerY)
    , m_centerLatitude(0)
    , m_radius(Config::radius())
    , m_timestamp(timestamp)
{
    if (Config::coordinateSystem() == Config::Metric) {
        m_centerLongitude = m_centerX;
        m_centerLatitude = m_centerY;
    } else {
        Utils::metersToLatLong(m_centerY, m_centerX, &m_centerLatitude, &m_centerLongitude);
    }
}

bool Disk::isPointInDisk(const std::shared_ptr<GPSPoint>& point)
{
    double distance = Utils::distance(m_centerX, m_centerY, point->longitudeMeters(), point->latitudeMeters());
    return Utils::fuzzyLessEqual(distance, m_radius);
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
