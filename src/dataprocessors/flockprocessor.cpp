#include "flockprocessor.h"

#include <iostream>
#include "componentfactory.h"
#include "disk.h"
#include "gpspoint.h"
#include "gpspointbuffererlistenerdata.h"
#include "utils.h"

void FlockProcessor::processData(const ProcessorData& data)
{
    const GPSPointBuffererListenerData& gpsData = static_cast<const GPSPointBuffererListenerData&>(data);
    const auto& points = gpsData.m_points;
    if (points.size() < m_flockManager.trajectoriesPerFlock())
        return;

    const uint64_t& timestamp = gpsData.m_timestamp;
    m_flockUtils->setFutureBufferSize(gpsData.m_extent);
    m_flockUtils->setCurrentTimeslot(timestamp);
    for (const auto& pointMapVectorEntry : points) {
        const auto& pointEntries = pointMapVectorEntry.second;
        m_flockUtils->cachePointPresence(pointMapVectorEntry.first, gpsData.m_presenceMap.at(pointMapVectorEntry.first));
        for (const auto& point : pointEntries)
            m_gridManager.addPointToGrid(point);
    }

    onGridsReady();

    m_flockManager.tryMergeFlocks(m_diskManager.disks(), timestamp);
    std::vector<Flock> flocks = m_flockManager.reportFlocks();
    m_flocks.insert(m_flocks.end(), flocks.begin(), flocks.end());
    m_flockUtils->clearCache();
    // Clear the grid, we don't need it anymore.
    m_gridManager.clear();
    m_diskManager.clear();
}

void FlockProcessor::onGridsReady()
{
    const auto& grids = m_gridManager.grids();
    for (const auto& gridPair : grids) {
        const auto& extendedGrid = m_gridManager.extendedGrid(gridPair.first);
        if (extendedGrid.size() < m_flockManager.trajectoriesPerFlock())
            continue;

        m_flockUtils->processExtendedGrid(extendedGrid, [&](const std::shared_ptr<Disk>& disk)
            {
                m_diskManager.tryInsertDisk(disk);
            }
        );
    }
}

void FlockProcessor::onDataProviderFinished()
{
    std::cout << "Flocks found: " << m_flocks.size() << std::endl;
    m_flockManager.clear();
    endDataHandler();
}

REGISTER_DATA_PROCESSOR("flockprocessor", "f", FlockProcessor::instance);

