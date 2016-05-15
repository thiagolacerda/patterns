#ifndef MANAGER_H
#define MANAGER_H

#include <memory>
#include <vector>
#include <unordered_map>
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
        , m_buffered(0)
        , m_bufferStart(UINT64_MAX)
    {
        m_mask = (1 << Config::flockLength()) - 1;
    }

    ~Manager();

    void start();
    void dumpFoundFlocks() const;
    int foundFlocks() const { return m_flocks.size(); }
    void addSequenceEntry(uint32_t trajectoryId);

private:
    bool shouldInsert(uint32_t trajectoryId);
    void shiftBuffer();
    void flushFlocksToResultFile();
    void validateAndTryStoreDisk(Disk* disk);
    void computeFlocks(const std::unordered_map<uint32_t,
        std::vector<std::shared_ptr<GPSPoint>>>& points, uint64_t timestamp);
    void clusterPointsIntoDisks(Disk* disk1, Disk* disk2, const std::vector<std::shared_ptr<GPSPoint>>& pointsToProcess);
    DatabaseDecoder* m_dbDecoder;
    DiskManager m_diskManager;
    GridManager m_gridManager;
    FlockManager m_flockManager;
    GPSPointTemporalProcessor m_pointProcessor;
    std::vector<Flock> m_flocks;
    std::unordered_map<uint32_t, uint64_t> m_sequences;
    std::unordered_map<uint32_t, bool> m_shouldInsertCache;
    uint64_t m_buffered;
    uint64_t m_bufferStart;
    uint64_t m_mask;
    uint64_t m_bufferMask;
};

#endif  // MANAGER_H
