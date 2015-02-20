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

    m_dbDecoder->setGPSTupleListener(&m_pointProcessor);
    m_dbDecoder->retrievePoints();
    m_pointProcessor.postProcessPoints();
    if (Config::timeSlotSize() <= Config::flockLength()) {
        std::map<unsigned, std::vector<std::shared_ptr<GPSPoint>>> pointsPerTimeSlot = m_pointProcessor.pointsPerTimeSlot();
        m_pointProcessor.releasePoints();
        for (auto iter = pointsPerTimeSlot.begin(); iter != pointsPerTimeSlot.end();) {
            // For each time instance, we get all points belonging to that and try to find flocks
            computeFlocks(iter->second, iter->first);
            // They can be discarded after that, all points were already computed
            iter = pointsPerTimeSlot.erase(iter);
        }
    } else {
        std::cout << "Time slot size does not allow flocks, given the flock length"
            << std::endl;
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
