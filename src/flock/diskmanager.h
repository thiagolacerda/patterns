#ifndef DISKMANAGER_H
#define DISKMANAGER_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class Disk;
class GPSPoint;

class DiskManager {
public:
    DiskManager()
    { }

    void computeDisks(GPSPoint* point1, GPSPoint* point2, uint32_t timestamp, Disk** disk1, Disk** disk2);
    bool tryInsertDisk(Disk* disk);
    const std::vector<Disk*>& disks() const { return m_disks; }
    void clear();
    void dump() const;

private:
    void getDisks(GPSPoint* point1, GPSPoint* point2, uint32_t timestamp, Disk** disk1, Disk** disk2);
    void getDisksPaperVersion(GPSPoint* point1, GPSPoint* point2, uint32_t timestamp, Disk** disk1, Disk** disk2);
    std::string diskKey(double x, double y);
    std::vector<Disk*> m_disks;
};

#endif  // DISKMANAGER_H
