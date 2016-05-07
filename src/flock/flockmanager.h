#pragma once

#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "flock.h"

class Disk;
class Trajectory;
#if !defined(NEWDESIGN)
class Manager;
#endif

class FlockManager {
public:
    FlockManager()
#if defined(NEWDESIGN)
        : m_trajectoriesPerFlock(0)
        , m_flockLength(0)
        , m_buffered(0)
        , m_lastTimestamp(0)
#else
        : m_manager(nullptr)
#endif
    { }

    uint32_t intersection(const Flock& set1, Disk* disk, std::map<uint32_t, Trajectory>* inter);
    uint32_t intersection(const Flock& flock1, const Flock& flock2);
#if defined(NEWDESIGN)
    void tryMergeFlocks(const std::vector<Disk*>& disks, uint64_t timestamp);
#else
    void tryMergeFlocks(const std::vector<Disk*>& disks);
#endif
    std::vector<Flock> reportFlocks();
    uint32_t size() const { return m_flocks.size(); }
#if defined(NEWDESIGN)
    void clear();
    uint64_t pointPresence(uint32_t trajectoryId) { return m_presenceMap[trajectoryId]; }
    uint32_t trajectoriesPerFlock() const { return m_trajectoriesPerFlock; }
    void setTrajectoriesPerFlock(uint32_t n) { m_trajectoriesPerFlock = n; }
    uint32_t flockLength() const { return m_flockLength; }
    void setFlockLength(uint32_t l) { m_flockLength = l; }
    uint32_t bufferSize() const { return m_buffered; }
    void preProcessing(uint64_t timestamp);
#else
    void clear() { m_flocks.clear(); }
    void setManager(Manager* manager) { m_manager = manager; }
    void dump() const;
#endif

private:
    void updateBuffers(const Flock& flock);
    void mergeFlocks(std::vector<Flock>* flocks, const Flock& newFlock);
    void checkDuplicateAnswer();
#if defined(NEWDESIGN)
    void updatePresenceMap(uint32_t trajectoryId);
    void shiftPresenceMaps();
    std::unordered_map<uint32_t, uint64_t> m_presenceMap;
    uint32_t m_trajectoriesPerFlock;
    uint32_t m_flockLength;
    uint32_t m_buffered;
    uint64_t m_lastTimestamp;
#else
    Manager* m_manager;
#endif
    std::vector<Flock> m_flocks;
    std::unordered_set<uint32_t> m_updated;
};

