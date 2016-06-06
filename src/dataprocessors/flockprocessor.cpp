#include "flockprocessor.h"

#include <iostream>
#include "componentfactory.h"
#include "disk.h"
#include "gpspoint.h"
#include "gpspointlistenerdata.h"
#include "utils.h"

void FlockProcessor::processData(const ProcessorData& data)
{
    const GPSPointListenerData& gpsData = static_cast<const GPSPointListenerData&>(data);
    const auto& points = gpsData.m_points;
    if (points.size() < m_flockManager.trajectoriesPerFlock())
        return;

    const auto& extraData = gpsData.m_extra;
    const uint64_t& timestamp = gpsData.m_timestamp;
    if (extraData)
        m_flockUtils->setFutureBufferSize(extraData->m_extent);

    m_flockUtils->setCurrentTimeslot(timestamp);

    for (const auto& pointMapVectorEntry : points) {
        const auto& pointEntries = pointMapVectorEntry.second;
        if (extraData)
            m_flockUtils->cachePointPresence(pointMapVectorEntry.first, extraData->m_presenceMap.at(pointMapVectorEntry.first));
        for (const auto& point : pointEntries)
            m_gridManager.addPointToGrid(point);
    }

    onGridsReady();

    m_flockManager.tryMergeFlocks(m_diskManager.disks(), timestamp);
    std::vector<Flock> flocks = m_flockManager.reportFlocks();
    m_flocks.insert(m_flocks.end(), flocks.begin(), flocks.end());
    m_flockUtils->clearCache();
    if (m_logger)
        m_logger->log(std::to_string(timestamp) + ";" + std::to_string(m_diskManager.size()) + ";" +
            std::to_string(m_flockManager.size()));

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

