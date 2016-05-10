#include "parallelflockprocessor.h"

#include <algorithm>
#include "componentfactory.h"

ParallelFlockProcessor::~ParallelFlockProcessor()
{
    std::for_each(m_futures.begin(), m_futures.end(), [](FlockProcessorThread* f) { delete f; });
    m_futures.clear();
}

void ParallelFlockProcessor::onGridsReady()
{
    m_buffer.reset();
    for (auto* f : m_futures)
        f->start();

    const auto& grids = m_gridManager.grids();
    for (const auto& gridPair : grids) {
        const auto& extendedGrid = m_gridManager.extendedGrid(gridPair.first);
        if (extendedGrid.size() < m_flockManager.trajectoriesPerFlock())
            continue;

        m_buffer.enqueue(extendedGrid);
    }

    m_buffer.end();

    for (auto& f : m_futures) {
        std::vector<std::shared_ptr<Disk>> disks = f->get();
        for (const auto& disk : disks)
            m_diskManager.tryInsertDisk(disk);
    }
}

REGISTER_DATA_PROCESSOR("pflockprocessor", "pf", ParallelFlockProcessor::instance);

