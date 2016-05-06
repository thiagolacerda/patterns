#include "manager.h"

#include <fstream>
#include <iostream>
#include <string>
#include "databasedecoder.h"
#include "disk.h"
#include "factory.h"
#include "gpspoint.h"
#include "grid.h"
#include "memoryreporter.h"
#include "performancelogger.h"
#include "trajectory.h"
#include "utils.h"

Manager::~Manager()
{
    if (m_dbDecoder)
        delete m_dbDecoder;
}

void Manager::start()
{
    m_dbDecoder = Factory::dbDecoderInstance(Config::decoder());
    if (!m_dbDecoder)
        return;

    m_flockManager.setManager(this);
    if (Config::onlineProcessing())
        m_pointProcessor.registerCallback(std::bind(&Manager::computeFlocks, this, std::placeholders::_1, std::placeholders::_2));

    if (Config::reportPerformance())
        PerformanceLogger::initLogger("Timestamp;Memory;Disks;Flocks");

    m_dbDecoder->setListenerFunction(std::bind(&GPSPointTemporalProcessor::processGPSTuple, &m_pointProcessor, std::placeholders::_1));
    m_dbDecoder->retrievePoints();
    m_dbDecoder->done();
    if (!Config::onlineProcessing()) {
        auto pointsPerTimeSlot = m_pointProcessor.pointsPerTimeSlot();
        m_pointProcessor.releasePoints();
        for (auto iter = pointsPerTimeSlot.begin(); iter != pointsPerTimeSlot.end();) {
            // For each time instance, we get all points belonging to that and try to find flocks
            computeFlocks(iter->second, iter->first);
            // They can be discarded after that, all points were already computed
            iter = pointsPerTimeSlot.erase(iter);
        }
    } else {
        m_pointProcessor.complete();
    }
    std::cout << "Flocks found: " << m_flocks.size() << std::endl;
    if (Config::flushFlocksToFile())
        flushFlocksToResultFile();
}

void Manager::dumpFoundFlocks() const
{
    for (const Flock& flock : m_flocks) {
        flock.dump();
        flock.dumpTrajectories();
    }
}

bool Manager::shouldInsert(uint32_t trajectoryId)
{
    if (!Config::onlineProcessing() || !Config::buffering())
        return true;

    const auto& iter = m_shouldInsertCache.find(trajectoryId);
    if (iter != m_shouldInsertCache.end())
        return iter->second;

    uint64_t mask = m_mask;
    uint64_t pointProcessorBuffer = m_pointProcessor.buffered();
    uint64_t size = pointProcessorBuffer + m_buffered;
    if (size < Config::flockLength())
        mask = m_mask >> (Config::flockLength() - size);

    uint64_t bufferCheck = (m_pointProcessor.getMask(trajectoryId) << m_buffered) | (m_bufferMask & m_sequences[trajectoryId]);
    uint64_t checks = std::max(uint64_t(1), size - Config::flockLength() + 1);
    while (checks > 0) {
        if (((bufferCheck & mask) ^ mask) == 0) {
            m_shouldInsertCache.emplace(std::make_pair(trajectoryId, true));
            return true;
        }

        mask = mask << 1;
        --checks;
    }
    m_shouldInsertCache.emplace(std::make_pair(trajectoryId, false));
    return false;
}

void Manager::computeFlocks(const std::unordered_map<uint32_t, std::vector<std::shared_ptr<GPSPoint>>>& points, uint64_t timestamp)
{
    // If the number of points to be processed for this time instance is less than the minimum number of trajectories
    // per flock, than we don't need to go further
    if (points.size() < Config::numberOfTrajectoriesPerFlock())
        return;

    if (Config::onlineProcessing() && Config::buffering()) {
        if (m_bufferStart == UINT64_MAX || (timestamp - (m_bufferStart + m_buffered - 1)) > 1) {
            m_bufferStart = timestamp;
            m_buffered = 0;
            m_sequences.clear();
            m_flockManager.clear();
        } else if (m_buffered >= Config::flockLength()) {
            shiftBuffer();
            ++m_bufferStart;
            --m_buffered;
        }
    }
    m_bufferMask = (1 << m_buffered) - 1;
    // Build the grid for this time slot
    for (const auto& pointMapVectorEntry : points) {
        const auto& pointEntries = pointMapVectorEntry.second;
        for (const std::shared_ptr<GPSPoint>& point : pointEntries)
            m_gridManager.addPointToGrid(std::shared_ptr<GPSPoint>(point));
    }

    Disk* disk1;
    Disk* disk2;
    const std::unordered_map<std::string, Grid*>& grids = m_gridManager.grids();
    const double& gridSize = Config::gridSize();
    for (auto iter = grids.begin(); iter != grids.end(); ++iter) {
        const std::string& key = iter->first;
        std::vector<std::shared_ptr<GPSPoint>> pointsToProcess;
        m_gridManager.extendedGridPoints(key, &pointsToProcess);
        if (pointsToProcess.size() < Config::numberOfTrajectoriesPerFlock())
            continue;

        for (auto it1 = pointsToProcess.begin(); it1 != pointsToProcess.end(); ++it1) {
            for (auto it2 = std::next(it1); it2 != pointsToProcess.end(); ++it2) {
                if ((*it1)->trajectoryId() == (*it2)->trajectoryId())
                    continue;

                double distance = (*it1)->distanceToPoint(*(*it2));
                if (Utils::fuzzyLessEqual(distance, gridSize)) {
                    disk1 = nullptr;
                    disk2 = nullptr;
                    m_diskManager.computeDisks((*it1).get(), (*it2).get(), timestamp, &disk1, &disk2);
                    if (!disk1 || !disk2)
                        continue;

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
    m_shouldInsertCache.clear();
    if (Config::reportPerformance()) {
        PerformanceLogger::log(std::to_string(timestamp) + ";" +
            std::to_string(MemoryReporter::getMemoryConsumption()) + ";" + std::to_string(m_diskManager.size()) + ";" +
            std::to_string(m_flockManager.size()));
    }
    // Clear the grid, we don't need it anymore.
    m_gridManager.clear();
    m_diskManager.clear();
    ++m_buffered;
}

void Manager::shiftBuffer()
{
    for (auto iter = m_sequences.begin(); iter != m_sequences.end();) {
        if (iter->second) {
            iter->second >>= 1;
            ++iter;
        } else {
            iter = m_sequences.erase(iter);
        }
    }
}

void Manager::addSequenceEntry(uint32_t trajectoryId)
{
    m_sequences[trajectoryId] |= (1 << m_buffered);
}

void Manager::flushFlocksToResultFile()
{
    std::string fileName = m_dbDecoder->decoderName() + "_n" + std::to_string(Config::numberOfTrajectoriesPerFlock()) +
        "_l" + std::to_string(Config::flockLength()) + "_g" + Utils::toString<double>(Config::gridSize(), 2) + "_t" +
        Utils::toString<double>(Config::timeSlotSize(), 2) + (Config::interpolate() ? "_i" : "") +
        (Config::outlierSpeedCutOff() != -1 ? ("_o" + Utils::toString(Config::outlierSpeedCutOff(), 2)) : "") + ".txt";

    std::ofstream out(fileName, std::ofstream::out);
    for (const Flock& f : m_flocks) {
        const auto& trajectories = f.trajectories();
        for (const auto& trajectoryPair : trajectories) {
            const auto& points = trajectoryPair.second.points();
            for (const auto& point : points) {
                out << f.id() << ";" << point->trajectoryId() << ";" << point->latitude() << ";" <<
                    point->longitude() << ";" << point->timestamp() << "\n";
            }
        }
    }
}

/*
 * Checks if the disk fulfills the requirements to be a potential new flock
 */
void Manager::validateAndTryStoreDisk(Disk* disk)
{
    if (disk->numberOfPoints() < Config::numberOfTrajectoriesPerFlock() || !m_diskManager.tryInsertDisk(disk))
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
    for (const std::shared_ptr<GPSPoint>& point : pointsToProcess) {
        bool toInsert = shouldInsert(point->trajectoryId());
        if (!toInsert)
            continue;

        if (point.get() == diskGeneratorPoint1 || point.get() == diskGeneratorPoint2) {
            disk1->addPoint(point);
            disk2->addPoint(point);
        } else {
            if (disk1->isPointInDisk(point))
                disk1->addPoint(point);

            if (disk2->isPointInDisk(point))
                disk2->addPoint(point);
        }
    }
}

