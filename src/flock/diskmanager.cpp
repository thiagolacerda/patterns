#include "diskmanager.h"

#include "disk.h"
#include "utils.h"

void DiskManager::tryInsertDisk(const std::shared_ptr<Disk>& disk)
{
    for (auto iter = m_disks.begin(); iter != m_disks.end();) {
        //TODO: figure out why using the function defined in Utils here causes so much degradation in performance
        uint32_t count = (*iter)->countIntersection(disk);
        if (disk->numberOfPoints() == count)
            return; // disk is a subset of disk already in disk manager, do not insert

        if ((*iter)->numberOfPoints() != count)
            ++iter; // Different disks... do nothing
        else
            iter = m_disks.erase(iter); // disk is a superset of *iter
    }

    m_disks.push_back(disk);
}

void DiskManager::clear()
{
    m_disks.clear();
}

