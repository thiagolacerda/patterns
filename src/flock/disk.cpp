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

void Disk::addTrajectory(const Trajectory& trajectory)
{
    m_trajectories[trajectory.id()] = trajectory;
}

uint32_t Disk::countIntersection(Disk* other) const
{
    auto begin1 = m_trajectories.begin();
    auto begin2 = other->m_trajectories.begin();
    auto end1 = m_trajectories.end();
    auto end2 = other->m_trajectories.end();
    --end1;
    --end2;
    if ((begin1->first < begin2->first && end1->first < begin2->first) ||
        (begin2->first < begin1->first && end2->first < begin1->first))
        return 0;

    uint32_t count = 0;
    while (begin1 != m_trajectories.end() && begin2 != other->m_trajectories.end()) {
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

void Disk::dumpTrajectories() const
{
    if (m_trajectories.empty())
        return;

    auto iter = m_trajectories.begin();
    std::cout << iter->second.id();
    ++iter;
    for (; iter != m_trajectories.end(); ++iter)
        std::cout << ", " << iter->second.id();
    std::cout << std::endl;
}

void Disk::dump() const
{
    std::cout << "Disk, x: " << m_centerX << ", y: " << m_centerY << " trajectories: "
        << m_trajectories.size() << std::endl;
}
