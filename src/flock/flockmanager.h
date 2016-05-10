#pragma once

#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "flock.h"
#if defined(NEWDESIGN)
#include "flockutils.h"
#endif

class Disk;
class Trajectory;
#if !defined(NEWDESIGN)
class Manager;
#endif

class FlockManager {
public:
#if defined(NEWDESIGN)
    FlockManager(const std::shared_ptr<FlockUtils>& flockUtils)
        : m_trajectoriesPerFlock(0)
        , m_flockLength(0)
        , m_buffered(0)
        , m_lastTimestamp(0)
        , m_flockUtils(flockUtils)
#else
    FlockManager()
        : m_manager(nullptr)
#endif
    { }

    uint32_t intersection(const Flock& set1, const std::shared_ptr<Disk>& disk, std::map<uint32_t, Trajectory>* inter);
    uint32_t intersection(const Flock& flock1, const Flock& flock2);
#if defined(NEWDESIGN)
    void tryMergeFlocks(const std::vector<std::shared_ptr<Disk>>& disks, uint64_t timestamp);
#else
    void tryMergeFlocks(const std::vector<std::shared_ptr<Disk>>& disks);
#endif
    std::vector<Flock> reportFlocks();
    uint32_t size() const { return m_flocks.size(); }
#if defined(NEWDESIGN)
    void clear();
    uint32_t trajectoriesPerFlock() const { return m_trajectoriesPerFlock; }
    void setTrajectoriesPerFlock(uint32_t n) { m_trajectoriesPerFlock = n; }
    uint32_t flockLength() const { return m_flockLength; }
    void setFlockLength(uint32_t l) { m_flockLength = l; }
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
    uint32_t m_trajectoriesPerFlock;
    uint32_t m_flockLength;
    uint32_t m_buffered;
    uint64_t m_lastTimestamp;
    std::shared_ptr<FlockUtils> m_flockUtils;
#else
    Manager* m_manager;
#endif
    std::vector<Flock> m_flocks;
    std::unordered_set<uint32_t> m_updated;
};

