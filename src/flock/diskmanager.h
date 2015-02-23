#ifndef DiskManager_h
#define DiskManager_h

#include <memory>
#include <string>
#include <vector>

class Disk;
class GPSPoint;

class DiskManager {
public:
    DiskManager()
    { }

    void computeDisks(GPSPoint* point1, GPSPoint* point2, unsigned long timestamp, Disk** disk1, Disk** disk2);
    bool tryInsertDisk(Disk*);
    const std::vector<Disk*>& disks() const { return m_disks; }
    void clear();
    void dump() const;
    unsigned numberOfDisks() const { return m_disks.size(); }

private:
    void getDisks(GPSPoint* point1, GPSPoint* point2, unsigned long timestamp, Disk** disk1, Disk** disk2);
    void getDisksPaperVersion(GPSPoint* point1, GPSPoint* point2, unsigned long timestamp, Disk** disk1, Disk** disk2);
    std::string diskKey(double x, double y);
    std::vector<Disk*> m_disks;
};

#endif // DiskManager_h
