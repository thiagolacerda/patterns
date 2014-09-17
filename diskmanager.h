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
    void clear();

private:
    std::string diskKey(double x, double y);
    std::vector<Disk*> m_disks;
};

#endif // DiskManager_h
