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

    uint32_t intersection(const std::map<uint32_t, Trajectory>& set1,
        const std::map<uint32_t, Trajectory>& set2, std::map<uint32_t, Trajectory>* inter);
    void tryMergeFlocks(const std::vector<Disk*>&);
    std::vector<Flock> reportFlocks();
    void dump() const;
private:
    bool mergeFlocks(std::vector<Flock>* flocks, const Flock& newFlock);
    void checkDuplicateAnswer();
    std::vector<Flock> m_flocks;
};

#endif  // FLOCKMANAGER_H
