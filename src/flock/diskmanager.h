#pragma once

#include <cstdint>
#include <memory>
#include <vector>

class Disk;
class GPSPoint;

class DiskManager {
public:
    DiskManager()
#if defined(NEWDESIGN)
        : m_radius(0)
#endif
    { }

    void computeDisks(GPSPoint* point1, GPSPoint* point2, uint64_t timestamp, std::shared_ptr<Disk>& disk1,
        std::shared_ptr<Disk>& disk2);
    void tryInsertDisk(const std::shared_ptr<Disk>& disk);
    const std::vector<std::shared_ptr<Disk>>& disks() const { return m_disks; }
    uint32_t size() const { return m_disks.size(); }
    void clear();
#if defined(NEWDESIGN)
    void setRadius(double radius) { m_radius = radius; }
#else
    void dump() const;
#endif

private:
#if !defined(NEWDESIGN)
    void getDisks(GPSPoint* point1, GPSPoint* point2, uint64_t timestamp, std::shared_ptr<Disk>& disk1,
        std::shared_ptr<Disk>& disk2);
    void getDisksPaperVersion(GPSPoint* point1, GPSPoint* point2, uint64_t timestamp, std::shared_ptr<Disk>& disk1,
        std::shared_ptr<Disk>& disk2);
#else
    double m_radius;
#endif
    std::vector<std::shared_ptr<Disk>> m_disks;
};

