#pragma once

#include <cstdint>
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

    void computeDisks(GPSPoint* point1, GPSPoint* point2, uint64_t timestamp, Disk** disk1, Disk** disk2);
    bool tryInsertDisk(Disk* disk);
    const std::vector<Disk*>& disks() const { return m_disks; }
    uint32_t size() const { return m_disks.size(); }
    void clear();
#if defined(NEWDESIGN)
    void setRadius(double radius) { m_radius = radius; }
#else
    void dump() const;
#endif

private:
#if !defined(NEWDESIGN)
    void getDisks(GPSPoint* point1, GPSPoint* point2, uint64_t timestamp, Disk** disk1, Disk** disk2);
    void getDisksPaperVersion(GPSPoint* point1, GPSPoint* point2, uint64_t timestamp, Disk** disk1, Disk** disk2);
#else
    double m_radius;
#endif
    std::vector<Disk*> m_disks;
};

