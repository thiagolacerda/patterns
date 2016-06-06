#include "disk.h"

#include "gpspoint.h"
#include "utils.h"

Disk::Disk()
    : Disk(0, 0, 0, 0, 0, 0)
{
}

Disk::Disk(double radius, double centerX, double centerY, uint64_t timestamp, uint32_t p1Id, uint32_t p2Id)
    : m_centerX(centerX)
    , m_centerLongitude(0)
    , m_centerY(centerY)
    , m_centerLatitude(0)
    , m_radius(radius)
    , m_timestamp(timestamp)
    , m_p1Id(p1Id)
    , m_p2Id(p2Id)
{
}

bool Disk::isPointInDisk(const std::shared_ptr<GPSPoint>& point)
{
    return isPointInDisk(point->trajectoryId(), point->longitudeMeters(), point->latitudeMeters());
}

bool Disk::isPointInDisk(uint32_t id, double x, double y) const
{
    return id == m_p1Id || id == m_p2Id || Utils::fuzzyLessEqual(Utils::distance(m_centerX, m_centerY, x, y), m_radius);
}

void Disk::addPoint(const std::shared_ptr<GPSPoint>& point)
{
    m_points[point->trajectoryId()] = point;
}

uint32_t Disk::countIntersection(const std::shared_ptr<Disk>& other) const
{
    return Utils::intersection(m_points.begin(), m_points.end(), other->m_points.begin(), other->m_points.end());
}

