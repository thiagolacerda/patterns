#pragma once

#include <cstdint>
#include <memory>
#include <vector>

class Disk;

class DiskManager {
public:
    DiskManager()
    { }

    void tryInsertDisk(const std::shared_ptr<Disk>& disk);
    const std::vector<std::shared_ptr<Disk>>& disks() const { return m_disks; }
    uint32_t size() const { return m_disks.size(); }
    void clear();

private:
    std::vector<std::shared_ptr<Disk>> m_disks;
};

