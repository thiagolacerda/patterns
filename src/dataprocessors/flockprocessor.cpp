#include "flockprocessor.h"

#include <iostream>
#include "componentfactory.h"
#include "disk.h"
#include "gpspoint.h"
#include "gpspointbuffererlistener.h"
#include "gpspointbuffererlistenerdata.h"
#include "utils.h"

bool FlockProcessor::shouldInsert(uint32_t trajectoryId, const GPSPointBuffererListenerData& data)
{
    const auto& iter = m_shouldInsertCache.find(trajectoryId);
    if (iter != m_shouldInsertCache.end())
        return iter->second;

    uint64_t mask = m_mask;
    uint32_t buffered = m_flockManager.bufferSize();
    uint64_t listenerBuffer = data.m_extent;
    uint64_t size = listenerBuffer + buffered;
    uint32_t flockLength = m_flockManager.flockLength();
    if (size < flockLength)
        mask = m_mask >> (flockLength - size);

    uint64_t fromList = data.m_presenceMap.at(trajectoryId);
    uint64_t fromFlock = m_flockManager.pointPresence(trajectoryId);
    uint64_t bufferCheck = (fromList << buffered) |
        (m_bufferMask & fromFlock);

    uint64_t checks = std::max(uint64_t(1), size - flockLength + 1);
    while (checks > 0) {
        if (((bufferCheck & mask) ^ mask) == 0) {
            m_shouldInsertCache.emplace(std::make_pair(trajectoryId, true));
            return true;
        }

        mask = mask << 1;
        --checks;
    }
    m_shouldInsertCache.emplace(std::make_pair(trajectoryId, false));
    return false;
}

void FlockProcessor::processData(const ProcessorData& data)
{
    const GPSPointBuffererListenerData& gpsData = static_cast<const GPSPointBuffererListenerData&>(data);
    const auto& points = gpsData.m_points;
    if (points.size() < m_flockManager.trajectoriesPerFlock())
        return;

    for (const auto& pointMapVectorEntry : points) {
        const auto& pointEntries = pointMapVectorEntry.second;
        for (const auto& point : pointEntries)
            m_gridManager.addPointToGrid(point);
    }

    m_bufferMask = (1 << m_flockManager.bufferSize()) - 1;
    const auto& grids = m_gridManager.grids();
    const double& gridSize = m_gridManager.gridSize();
    const auto& timestamp = gpsData.m_timestamp;
    for (const auto& gridPair : grids) {
        const auto& extendedGrid = m_gridManager.extendedGrid(gridPair.first);
        if (extendedGrid.size() < m_flockManager.trajectoriesPerFlock())
            continue;

        for (auto it1 = extendedGrid.begin(); it1 != extendedGrid.end(); ++it1) {
            const uint32_t p1Id = (*it1)->trajectoryId();
            for (auto it2 = std::next(it1); it2 != extendedGrid.end(); ++it2) {
                const uint32_t p2Id = (*it2)->trajectoryId();
                if (p1Id == p2Id)
                    continue;

                double distance = (*it1)->distanceToPoint(*(*it2));
                if (Utils::fuzzyLessEqual(distance, gridSize)) {
                    std::shared_ptr<Disk> disk1;
                    std::shared_ptr<Disk> disk2;
                    m_diskManager.computeDisks((*it1).get(), (*it2).get(), timestamp, disk1, disk2);
                    if (!disk1 || !disk2)
                        continue;

                    clusterPointsIntoDisks(disk1, disk2, extendedGrid, gpsData);
                    if (disk1->numberOfPoints() >= m_flockManager.trajectoriesPerFlock())
                        m_diskManager.tryInsertDisk(disk1);

                    if (disk2->numberOfPoints() >= m_flockManager.trajectoriesPerFlock())
                        m_diskManager.tryInsertDisk(disk2);
                }
            }
        }
    }

    m_flockManager.tryMergeFlocks(m_diskManager.disks(), timestamp);
    std::vector<Flock> flocks = m_flockManager.reportFlocks();
    m_flocks.insert(m_flocks.end(), flocks.begin(), flocks.end());
    m_shouldInsertCache.clear();
    // Clear the grid, we don't need it anymore.
    m_gridManager.clear();
    m_diskManager.clear();
}

void FlockProcessor::clusterPointsIntoDisks(const std::shared_ptr<Disk>& disk1, const std::shared_ptr<Disk>& disk2,
    const std::vector<std::shared_ptr<GPSPoint>>& points, const GPSPointBuffererListenerData& data)
{
    for (const auto& point : points) {
        const uint32_t id = point->trajectoryId();
        bool toInsert = shouldInsert(id, data);
        if (!toInsert)
            continue;

        const double x = point->longitudeMeters();
        const double y = point->latitudeMeters();
        if (disk1->isPointInDisk(id, x, y))
            disk1->addPoint(point);

        if (disk2->isPointInDisk(id, x, y))
            disk2->addPoint(point);
    }
}

void FlockProcessor::onDataProviderFinished()
{
    std::cout << "Flocks found: " << m_flocks.size() << std::endl;
    m_flockManager.clear();
    endDataHandler();
}

REGISTER_DATA_PROCESSOR("flockprocessor", "f", FlockProcessor::instance);

