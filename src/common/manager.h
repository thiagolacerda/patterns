#ifndef MANAGER_H
#define MANAGER_H

#include <map>
#include <memory>
#include <vector>
#include "config.h"
#include "diskmanager.h"
#include "flock.h"
#include "flockmanager.h"
#include "gpspointtemporalprocessor.h"
#include "gridmanager.h"

class DatabaseDecoder;
class Disk;
class Grid;

class Manager {
public:
    Manager()
        : m_dbDecoder(nullptr)
        , m_gridManager(GridManager(Config::gridSize()))
    { }

    void start();
    void dumpFoundFlocks() const;
    int foundFlocks() const { return m_flocks.size(); }

private:
    void validateAndTryStoreDisk(Disk* disk);
    void computeFlocks(const std::vector<std::shared_ptr<GPSPoint>>&, uint64_t timestamp);
    void clusterPointsIntoDisks(Disk* disk1, Disk* disk2, const std::vector<std::shared_ptr<GPSPoint>>& pointsToProcess,
        GPSPoint* diskGeneratorPoint1, GPSPoint* diskGeneratorPoint2);
    void clusterGridPoints(Disk* disk1, Disk* disk2, Grid* grid, GPSPoint* diskGeneratorPoint1,
        GPSPoint* diskGeneratorPoint2);
    void createTrajectoryAndAddToDisks(const std::shared_ptr<GPSPoint>& point, Disk* disk1, Disk* disk2 = nullptr);
    DatabaseDecoder* m_dbDecoder;
    DiskManager m_diskManager;
    GridManager m_gridManager;
    FlockManager m_flockManager;
    GPSPointTemporalProcessor m_pointProcessor;
    std::vector<Flock> m_flocks;
};

#endif  // MANAGER_H
