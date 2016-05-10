#include "flockprocessorthread.h"

#include "disk.h"
#include "gpspoint.h"
#include "sharedbuffer.h"
#include "utils.h"

void FlockProcessorThread::start()
{
    m_future = std::move(std::async(std::launch::async, &FlockProcessorThread::processGrids, this));
}

std::vector<std::shared_ptr<Disk>> FlockProcessorThread::get()
{
    return m_future.get();
}

std::vector<std::shared_ptr<Disk>> FlockProcessorThread::processGrids()
{
    m_disksBuffer.reset();
    std::future<std::vector<std::shared_ptr<Disk>>> disksFuture(std::async(std::launch::async,
        &FlockProcessorThread::processDisks, this));
    while (true) {
        const auto& gridVec = m_gridBuffer.dequeue();
        if (gridVec.empty())
            break;

        m_flockUtils->processExtendedGrid(gridVec, [&](const std::shared_ptr<Disk>& disk)
            {
                m_disksBuffer.enqueue(disk);
            }
        );
    }
    m_disksBuffer.end();
    return disksFuture.get();
}

std::vector<std::shared_ptr<Disk>> FlockProcessorThread::processDisks()
{
    m_diskManager.clear();
    while (true) {
        const auto& disk = m_disksBuffer.dequeue();
        if (!disk)
            break;

        m_diskManager.tryInsertDisk(disk);
    }
    return m_diskManager.disks();
}

