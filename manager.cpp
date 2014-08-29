#include "manager.h"

#include <iostream>
#include "databasedecoder.h"
#include "disk.h"
#include "factory.h"
#include "gpspoint.h"
#include "grid.h"
#include "trajectory.h"

void Manager::start()
{
    m_dbDecoder = Factory::dbDecoderInstance();
    if (!m_dbDecoder)
        return;

    m_dbDecoder->setGPSTupleListener(this);
    m_dbDecoder->retrievePoints();
    for (auto iter = m_pointsPerTimeSlot.begin(); iter != m_pointsPerTimeSlot.end();) {
        computeDisks(iter->second);
        iter = m_pointsPerTimeSlot.erase(iter);
    }
}

void Manager::processGPSTuple(const std::tuple<unsigned long, double, double, unsigned long>& tuple)
{
    unsigned long tID;
    double latitude;
    double longitude;
    unsigned long timestamp;
    std::tie(tID, latitude, longitude, timestamp) = tuple;
    unsigned index = timestamp / Config::timeSlotSize();

    if (Config::coordinateSystem() == Config::Cartesian)
        Utils::latLongToMeters(latitude, longitude, &latitude, &longitude);

    if (m_pointsPerTimeSlot.find(index) == m_pointsPerTimeSlot.end())
        m_pointsPerTimeSlot[index] = std::vector<std::shared_ptr<GPSPoint>>();

    m_pointsPerTimeSlot[index].push_back(std::shared_ptr<GPSPoint>(new GPSPoint(latitude, longitude, timestamp, tID)));
}

void Manager::computeDisks(const std::vector<std::shared_ptr<GPSPoint>>& points)
{
    // Build the grid for this time slot
    for (const std::shared_ptr<GPSPoint>& point : points)
        m_gridManager.addPointToGrid(std::shared_ptr<GPSPoint>(point));

    std::shared_ptr<Disk> disk1;
    std::shared_ptr<Disk> disk2;
    const std::unordered_map<std::string, std::shared_ptr<Grid>>& grids = m_gridManager.grids();
    double gridSize = Config::gridSize();
    unsigned trajectoriesPerFlock = Config::numberOfTrajectoriesPerFlock();
    for (auto iter = grids.begin(); iter != grids.end(); ++iter) {
        std::string key = iter->first;
        std::vector<Disk*> resultingDisks;
        std::vector<std::shared_ptr<GPSPoint>> pointsToProcess;
        std::vector<std::shared_ptr<Grid>> neighborGrids;
        m_gridManager.neighborGridsAndPoints(key, neighborGrids, pointsToProcess);
        const std::vector<std::shared_ptr<GPSPoint>>& gridPoints = iter->second->points();
        pointsToProcess.insert(pointsToProcess.end(), gridPoints.begin(), gridPoints.end());
        for (auto it1 = pointsToProcess.begin(); it1 != pointsToProcess.end(); ++it1) {
            for (auto it2 = std::next(it1); it2 != pointsToProcess.end(); ++it2) {
                double distance = (*it1)->distanceToPoint(*(*it2));
                if (distance <= gridSize) {
                    m_diskManager.computeDisks((*it1).get(), (*it2).get(), disk1, disk2);
                    getTrajectoryAndAddToDisks(*it1, disk1.get(), disk2.get());
                    getTrajectoryAndAddToDisks(*it2, disk1.get(), disk2.get());
                    clusterPointsIntoDisks(disk1.get(), disk2.get(), pointsToProcess, (*it1).get(), (*it2).get());
                    if (disk1->trajectories().size() >= trajectoriesPerFlock)
                        diskIsValid(disk1, iter->second, neighborGrids);
                    if (disk2->trajectories().size() >= trajectoriesPerFlock)
                        diskIsValid(disk2, iter->second, neighborGrids);
                }
            }
        }
    }
    // Clear the grid, we don't need it anymore.
    m_gridManager.clear();
}

void Manager::diskIsValid(const std::shared_ptr<Disk>& disk, const std::shared_ptr<Grid>& queryGrid,
    const std::vector<std::shared_ptr<Grid>>& neighborGrids)
{
    m_diskManager.addDisk(disk);
    disk->addAlreadyComputedGrids(neighborGrids);
    disk->addAlreadyComputedGrid(queryGrid);
}

void Manager::clusterPointsIntoDisks(Disk* disk1, Disk* disk2,
    const std::vector<std::shared_ptr<GPSPoint>>& pointsToProcess, GPSPoint* diskGeneratorPoint1,
    GPSPoint* diskGeneratorPoint2)
{
    double radius = Config::gridSize() / 2.0;
    for (std::shared_ptr<GPSPoint> point : pointsToProcess) {
        if (point.get() == diskGeneratorPoint1 || point.get() == diskGeneratorPoint2)
            continue;

        double latitude = point->latitude();
        double longitude = point->longitude();
        std::shared_ptr<Grid> grid = m_gridManager.gridThatPointBelongsTo(point);
        if (!disk1->isGridAlreadyComputed(grid) &&
            Utils::distance(disk1->centerX(), disk1->centerY(), latitude, longitude) <= radius)
            getTrajectoryAndAddToDisks(point, disk1);

        if (!disk2->isGridAlreadyComputed(grid) &&
            Utils::distance(disk2->centerX(), disk2->centerY(), latitude, longitude) <= radius)
            getTrajectoryAndAddToDisks(point, disk2);
    }
}

void Manager::getTrajectoryAndAddToDisks(const std::shared_ptr<GPSPoint>& point, Disk* disk1, Disk* disk2)
{
    std::shared_ptr<Trajectory> trajectory = m_trajectoryManager.trajectoryById(point->trajectoryId());
    // Here we create a new Trajectory for this point if one of the following conditions are met:
    // There is no Trajectory created for this point
    // Or if its shared_ptr's use_count is equal to 2. This means that onlye the above shared_ptr and
    // TrajectoryManager holds reference for it e no valid Disk. This lazy deletion is being performed to gain speed.
    if (!trajectory || trajectory.use_count() == 2) {
        trajectory.reset(new Trajectory(point->trajectoryId()));
        m_trajectoryManager.addTrajectory(trajectory);
    }
    trajectory->addPoint(point);
    disk1->addTrajectory(trajectory);
    if (disk2)
        disk2->addTrajectory(trajectory);
}

void Manager::dumpPointsMap()
{
    std::cout << "Number of time slots: " << m_pointsPerTimeSlot.size() << std::endl;
    for (auto iter = m_pointsPerTimeSlot.begin(); iter != m_pointsPerTimeSlot.end(); ++iter)
        std::cout << "\ttime slot: " << iter->first << ", number of points: " << iter->second.size() << std::endl;
}
