#ifndef FLOCKMANAGER_H
#define FLOCKMANAGER_H

#include <cstdint>
#include <map>
#include <unordered_map>
#include <vector>
#include "flock.h"

class Disk;
class Trajectory;

class FlockManager {
public:
    FlockManager()
    { }

    uint32_t intersection(const Flock& set1, Disk* disk, std::map<uint32_t, Trajectory>* inter);
    uint32_t intersection(const Flock& flock1, const Flock& flock2);
    void tryMergeFlocks(const std::vector<Disk*>&);
    std::vector<Flock> reportFlocks();
    void dump() const;
    uint32_t size() const { return m_flocks.size(); }
private:
    void mergeFlocks(std::vector<Flock>* flocks, const Flock& newFlock);
    void checkDuplicateAnswer();
    std::vector<Flock> m_flocks;
};

#endif  // FLOCKMANAGER_H
