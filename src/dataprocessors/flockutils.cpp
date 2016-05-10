#include "flockutils.h"

#include <cmath>
#include "disk.h"
#include "gpspoint.h"
#include "utils.h"

void FlockUtils::setRadius(double radius)
{
    m_radius = radius;
    m_gridSize = m_radius * 2;
}

void FlockUtils::setPastBufferSize(uint32_t pastBufferSize)
{
    m_pastBufferSize = pastBufferSize;
    m_currentMask = (1 << m_pastBufferSize) - 1;
}

void FlockUtils::setFutureBufferSize(uint32_t futureBufferSize)
{
    m_futureBufferSize = futureBufferSize;
}

void FlockUtils::setFlockLength(uint32_t flockLength)
{
    m_flockLength = flockLength;
    m_mask = (1 << m_flockLength) - 1;
}

void FlockUtils::updatePresenceMap(uint32_t trajectoryId)
{
    m_flockPresenceMap[trajectoryId] |= (1 << m_pastBufferSize);
}

void FlockUtils::setCurrentTimeslot(uint64_t timeSlot)
{
    if (timeSlot - m_currentTimeSlot > 1)
        m_flockPresenceMap.clear();

    m_currentTimeSlot = timeSlot;
}
void FlockUtils::cachePointPresence(uint32_t trajectoryId, uint64_t futurePresence)
{
    uint64_t mask = m_mask;
    uint64_t size = m_futureBufferSize + m_pastBufferSize;
    if (size < m_flockLength)
        mask = m_mask >> (m_flockLength - size);

    uint64_t pastPresence = 0;
    auto iter = m_flockPresenceMap.find(trajectoryId);
    if (iter != m_flockPresenceMap.end())
        pastPresence = m_flockPresenceMap.at(trajectoryId);

    uint64_t bufferCheck = (futurePresence << m_pastBufferSize) | (m_currentMask & pastPresence);

    uint64_t checks = std::max(uint64_t(1), size - m_flockLength + 1);
    while (checks > 0) {
        if (((bufferCheck & mask) ^ mask) == 0) {
            m_pointPresenceCache.emplace(std::make_pair(trajectoryId, true));
            return;
        }

        mask = mask << 1;
        --checks;
    }
    m_pointPresenceCache.emplace(std::make_pair(trajectoryId, false));
}

void FlockUtils::shiftPresenceMap()
{
    for (auto iter = m_flockPresenceMap.begin(); iter != m_flockPresenceMap.end();) {
        if (iter->second) {
            iter->second >>= 1;
            ++iter;
        } else {
            iter = m_flockPresenceMap.erase(iter);
        }
    }
}

void FlockUtils::processExtendedGrid(const std::vector<std::shared_ptr<GPSPoint>>& grid, const OnDiskReadyFunc& func)
{
    for (auto it1 = grid.begin(); it1 != grid.end(); ++it1) {
        const uint32_t p1Id = (*it1)->trajectoryId();
        for (auto it2 = std::next(it1); it2 != grid.end(); ++it2) {
            const uint32_t p2Id = (*it2)->trajectoryId();
            if (p1Id == p2Id)
                continue;

            double distance = (*it1)->distanceToPoint(*(*it2));
            if (Utils::fuzzyLessEqual(distance, m_gridSize)) {
                std::shared_ptr<Disk> disk1;
                std::shared_ptr<Disk> disk2;
                computeDisks((*it1).get(), (*it2).get(), disk1, disk2);
                if (!disk1 || !disk2)
                    continue;

                clusterPointsIntoDisks(disk1, disk2, grid);
                if (disk1->numberOfPoints() >= m_trajectoriesPerFlock)
                    func(disk1);

                if (disk2->numberOfPoints() >= m_trajectoriesPerFlock)
                    func(disk2);
            }
        }
    }
}

void FlockUtils::computeDisks(GPSPoint* point1, GPSPoint* point2, std::shared_ptr<Disk>& disk1,
    std::shared_ptr<Disk>& disk2)
{
    const double latMeters1 = point1->latitudeMeters();
    const double longMeters1 = point1->longitudeMeters();
    const double latMeters2 = point2->latitudeMeters();
    const double longMeters2 = point2->longitudeMeters();
    if (Utils::fuzzyEqual(latMeters1, latMeters2) && Utils::fuzzyEqual(longMeters1, longMeters2))
        return;

    double midX;
    double midY;
    double vectorX;
    double vectorY;
    double perpNormVectorX;
    double perpNormVectorY;
    Utils::midPoint(longMeters1, latMeters1, longMeters2, latMeters2, midX, midY);
    Utils::toVector(longMeters1, latMeters1, longMeters2, latMeters2, vectorX, vectorY);
    double pointsDistance = Utils::vectorLength(vectorX, vectorY);
    double powMultiplyParameter = (m_radius * m_radius) - ((pointsDistance / 2.0) * (pointsDistance / 2.0));
    if (powMultiplyParameter < 0)
        return;

    Utils::normalizedVector(vectorY, -vectorX, perpNormVectorX, perpNormVectorY);
    double multiplyParameter = std::sqrt(powMultiplyParameter);
    double c1X = midX + multiplyParameter * perpNormVectorX;
    double c1Y = midY + multiplyParameter * perpNormVectorY;
    double c2X = midX - multiplyParameter * perpNormVectorX;
    double c2Y = midY - multiplyParameter * perpNormVectorY;
    disk1 = std::make_shared<Disk>(m_radius, c1X, c1Y, m_currentTimeSlot, point1->trajectoryId(), point2->trajectoryId());
    disk2 = std::make_shared<Disk>(m_radius, c2X, c2Y, m_currentTimeSlot, point1->trajectoryId(), point2->trajectoryId());
}

void FlockUtils::clusterPointsIntoDisks(const std::shared_ptr<Disk>& disk1, const std::shared_ptr<Disk>& disk2,
    const std::vector<std::shared_ptr<GPSPoint>>& points)
{
    for (const auto& point : points) {
        const uint32_t id = point->trajectoryId();
        if (!m_pointPresenceCache.at(id))
            continue;

        const double x = point->longitudeMeters();
        const double y = point->latitudeMeters();
        if (disk1->isPointInDisk(id, x, y))
            disk1->addPoint(point);

        if (disk2->isPointInDisk(id, x, y))
            disk2->addPoint(point);
    }
}

