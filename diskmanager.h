#ifndef DiskManager_h
#define DiskManager_h

#include <memory>
#include <string>
#include <unordered_map>

class Disk;
class GPSPoint;

class DiskManager {
public:
    DiskManager()
    { }

    void addDisk(const std::shared_ptr<Disk>&);
    std::shared_ptr<Disk> findDisk(double centerX, double centerY);
    void computeDisks(GPSPoint* point1, GPSPoint* point2, unsigned long timestamp, std::shared_ptr<Disk>& disk1,
        std::shared_ptr<Disk>& disk2);
    bool tryInsertDisk(const std::shared_ptr<Disk>&);

private:
    std::string diskKey(double x, double y);
    std::unordered_map<std::string, std::shared_ptr<Disk>> m_disks;
};

#endif // DiskManager_h
