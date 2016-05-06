#ifndef FLOCKMANAGER_H
#define FLOCKMANAGER_H

#include <cstdint>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "flock.h"

class Disk;
class Trajectory;
class Manager;

class FlockManager {
public:
    FlockManager()
        : m_manager(nullptr)
    { }

    void setManager(Manager* manager) { m_manager = manager; }
    uint32_t intersection(const Flock& set1, Disk* disk, std::map<uint32_t, Trajectory>* inter);
    uint32_t intersection(const Flock& flock1, const Flock& flock2);
    void tryMergeFlocks(const std::vector<Disk*>&);
    std::vector<Flock> reportFlocks();
    void dump() const;
    void clear() { m_flocks.clear(); }
    uint32_t size() const { return m_flocks.size(); }
private:
    void updateBuffers(const Flock& flock);
    void mergeFlocks(std::vector<Flock>* flocks, const Flock& newFlock);
    void checkDuplicateAnswer();
    std::vector<Flock> m_flocks;
    Manager* m_manager;
    std::unordered_set<uint32_t> m_updated;
};

#endif  // FLOCKMANAGER_H
