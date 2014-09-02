#ifndef Manager_h
#define Manager_h

#include <map>
#include <memory>
#include "config.h"
#include "diskmanager.h"
#include "gpstuplelistener.h"
#include "gridmanager.h"

class DatabaseDecoder;
class Disk;
class Grid;

class Manager : public GPSTupleListener {
public:
    Manager()
        : m_dbDecoder(nullptr)
        , m_gridManager(GridManager(Config::gridSize()))
    { }

    void start();
    void processGPSTuple(const std::tuple<unsigned long, double, double, unsigned long>&) override;
    void dumpPointsMap();

private:
    void validateAndTryStoreDisk(const std::shared_ptr<Disk>& disk, const std::shared_ptr<Grid>& queryGrid,
        const std::vector<std::shared_ptr<Grid>>& neighborGrids);
    void computeDisks(const std::vector<std::shared_ptr<GPSPoint>>&);
    void clusterPointsIntoDisks(Disk* disk1, Disk* disk2, const std::vector<std::shared_ptr<GPSPoint>>& pointsToProcess,
        GPSPoint* diskGeneratorPoint1, GPSPoint* diskGeneratorPoint2);
    void clusterGridPoints(Disk* disk1, Disk* disk2, Grid* grid, GPSPoint* diskGeneratorPoint1,
        GPSPoint* diskGeneratorPoint2);
    void createTrajectoryAndAddToDisks(const std::shared_ptr<GPSPoint>& point, Disk* disk1, Disk* disk2 = nullptr);
    DatabaseDecoder* m_dbDecoder;
    DiskManager m_diskManager;
    GridManager m_gridManager;
    std::map<unsigned, std::vector<std::shared_ptr<GPSPoint>>> m_pointsPerTimeSlot;
};

#endif // Manager_h
