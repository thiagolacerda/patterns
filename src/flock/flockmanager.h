#pragma once

#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "flock.h"
#include "flockutils.h"

class Disk;
class Trajectory;

class FlockManager {
public:
    FlockManager(const std::shared_ptr<FlockUtils>& flockUtils)
        : m_trajectoriesPerFlock(0)
        , m_flockLength(0)
        , m_buffered(0)
        , m_lastTimestamp(0)
        , m_flockUtils(flockUtils)
    { }

    void tryMergeFlocks(const std::vector<std::shared_ptr<Disk>>& disks, uint64_t timestamp);
    std::vector<Flock> reportFlocks();
    uint32_t size() const { return m_flocks.size(); }
    void clear();
    uint32_t trajectoriesPerFlock() const { return m_trajectoriesPerFlock; }
    void setTrajectoriesPerFlock(uint32_t n) { m_trajectoriesPerFlock = n; }
    uint32_t flockLength() const { return m_flockLength; }
    void setFlockLength(uint32_t l) { m_flockLength = l; }

private:
    void updateBuffers(const Flock& flock);
    void mergeFlocks(std::vector<Flock>* flocks, const Flock& newFlock);
    void checkDuplicateAnswer();
    uint32_t m_trajectoriesPerFlock;
    uint32_t m_flockLength;
    uint32_t m_buffered;
    uint64_t m_lastTimestamp;
    std::shared_ptr<FlockUtils> m_flockUtils;
    std::vector<Flock> m_flocks;
    std::unordered_set<uint32_t> m_updated;
};

