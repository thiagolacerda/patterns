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
    m_ignoreFirstPoint = Config::isInCompatibilityMode();
    m_dbDecoder->retrievePoints();
    for (auto iter = m_pointsPerTimeSlot.begin(); iter != m_pointsPerTimeSlot.end();) {
        // For each time instance, we get all points belonging to that and try to find flocks
        computeFlocks(iter->second, iter->first);
        // They can be discarded after that, all points were already computed
        iter = m_pointsPerTimeSlot.erase(iter);
    }
    std::cout << "Flocks found: " << m_flocks.size() << std::endl;
}

void Manager::dumpFoundFlocks() const
{
    for (const Flock& flock : m_flocks) {
        flock.dump();
        flock.dumpTrajectories();
    }
}

void Manager::processGPSTuple(const std::tuple<unsigned long, double, double, unsigned long>& tuple)
{
    if (m_ignoreFirstPoint) {
        m_ignoreFirstPoint = false;
        return;
    }

    unsigned long tID;
    double latitude;
    double longitude;
    unsigned long timestamp;
    std::tie(tID, latitude, longitude, timestamp) = tuple;
    unsigned index = Config::timeSlotSize() > 0 ? timestamp / Config::timeSlotSize() : timestamp;

    if (Config::coordinateSystem() == Config::Cartesian)
        Utils::latLongToMeters(latitude, longitude, &latitude, &longitude);

    if (m_pointsPerTimeSlot.find(index) == m_pointsPerTimeSlot.end())
        m_pointsPerTimeSlot[index] = std::vector<std::shared_ptr<GPSPoint>>();

    m_pointsPerTimeSlot[index].push_back(std::shared_ptr<GPSPoint>(new GPSPoint(latitude, longitude, timestamp, tID)));
}

void Manager::computeFlocks(const std::vector<std::shared_ptr<GPSPoint>>& points, unsigned timestamp)
{
    // If the number of points to be processed for this time instance is less than the minimum number of trajectories
    // per flock, than we don't need to go further
    if (points.size() < Config::numberOfTrajectoriesPerFlock())
        return;

    // Build the grid for this time slot
    for (const std::shared_ptr<GPSPoint>& point : points)
        m_gridManager.addPointToGrid(std::shared_ptr<GPSPoint>(point));

    Disk* disk1;
    Disk* disk2;
    const std::unordered_map<std::string, Grid*>& grids = m_gridManager.grids();
    double gridSize = Config::gridSize();
    for (auto iter = grids.begin(); iter != grids.end(); ++iter) {
        std::string key = iter->first;
        std::vector<std::shared_ptr<GPSPoint>> pointsToProcess;
        m_gridManager.extendedGridPoints(key, pointsToProcess);
        if (pointsToProcess.size() < Config::numberOfTrajectoriesPerFlock())
            continue;

        for (auto it1 = pointsToProcess.begin(); it1 != pointsToProcess.end(); ++it1) {
            for (auto it2 = std::next(it1); it2 != pointsToProcess.end(); ++it2) {
                double distance = (*it1)->distanceToPoint(*(*it2));
                if (Utils::fuzzyLessEqual(distance, gridSize)) {
                    disk1 = nullptr;
                    disk2 = nullptr;
                    if (Config::isInCompatibilityMode())
                        m_diskManager.computeDisksPaperVersion((*it1).get(), (*it2).get(), timestamp, &disk1, &disk2);
                    else
                        m_diskManager.computeDisks((*it1).get(), (*it2).get(), timestamp, &disk1, &disk2);
                    if (!disk1 || !disk2)
                        continue;

                    createTrajectoryAndAddToDisks(*it1, disk1, disk2);
                    createTrajectoryAndAddToDisks(*it2, disk1, disk2);
                    clusterPointsIntoDisks(disk1, disk2, pointsToProcess, (*it1).get(), (*it2).get());
                    validateAndTryStoreDisk(disk1);
                    validateAndTryStoreDisk(disk2);
                }
            }
        }
    }
    m_flockManager.tryMergeFlocks(m_diskManager.disks());
    std::vector<Flock> flocks = m_flockManager.reportFlocks();
    m_flocks.insert(m_flocks.end(), flocks.begin(), flocks.end());
    // Clear the grid, we don't need it anymore.
    m_gridManager.clear();
    m_diskManager.clear();
}

/*
 * Checks if the disk fulfil the requirements to be a potential new flock
 */
void Manager::validateAndTryStoreDisk(Disk* disk)
{
    if (disk->numberOfTrajectories() < Config::numberOfTrajectoriesPerFlock() || !m_diskManager.tryInsertDisk(disk))
        // Invalid disk, free it
        delete disk;
}


/*
 * This goes through all the points in the neighbor grids and try to put them inside the disks
 */
void Manager::clusterPointsIntoDisks(Disk* disk1, Disk* disk2,
    const std::vector<std::shared_ptr<GPSPoint>>& pointsToProcess, GPSPoint* diskGeneratorPoint1,
    GPSPoint* diskGeneratorPoint2)
{
    double radius = Config::radius();
    for (std::shared_ptr<GPSPoint> point : pointsToProcess) {
        if (point.get() == diskGeneratorPoint1 || point.get() == diskGeneratorPoint2)
            // diskGeneratorPoint1 and diskGeneratorPoint2 already belong to the disk
            continue;

        double latitude = point->latitude();
        double longitude = point->longitude();
        double distance = Utils::distance(disk1->centerX(), disk1->centerY(), latitude, longitude);
        if (Utils::fuzzyLessEqual(distance, radius))
            createTrajectoryAndAddToDisks(point, disk1);

        distance = Utils::distance(disk2->centerX(), disk2->centerY(), latitude, longitude);
        if (Utils::fuzzyLessEqual(distance, radius))
            createTrajectoryAndAddToDisks(point, disk2);
    }
}

void Manager::createTrajectoryAndAddToDisks(const std::shared_ptr<GPSPoint>& point, Disk* disk1, Disk* disk2)
{
    Trajectory trajectory(point->trajectoryId());
    trajectory.addPoint(point);
    disk1->addTrajectory(trajectory);
    if (disk2)
        disk2->addTrajectory(trajectory);
}

void Manager::dumpPointsMap()
{
    std::cout << "Number of time slots: " << m_pointsPerTimeSlot.size() << std::endl;
    for (auto iter = m_pointsPerTimeSlot.begin(); iter != m_pointsPerTimeSlot.end(); ++iter) {
        std::cout << "\t***time slot: " << iter->first << ", number of points: " << iter->second.size() << std::endl;
        const std::vector<std::shared_ptr<GPSPoint>>& points = iter->second;
        for (const std::shared_ptr<GPSPoint>& point : points) {
            std::cout << "\t\t";
            point->dump();
        }
    }
}
