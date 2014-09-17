#ifndef FlockManager_h
#define FlockManager_h

#include <map>
#include <vector>
#include <unordered_map>
#include "flock.h"

class Disk;
class Trajectory;

class FlockManager {
public:
    FlockManager()
    { }

    unsigned intersection(const std::map<unsigned long, Trajectory>& set1,
        const std::map<unsigned long, Trajectory>& set2, std::map<unsigned long, Trajectory>* inter);
    void tryMergeFlocks(const std::vector<Disk*>&);
    bool mergeFlocks(std::vector<Flock>& flocks, const Flock& newFlock);
    std::vector<Flock> reportFlocks();
    unsigned numberOfFlocks() const { return m_flocks.size(); }
    void dump() const;
private:
    void checkDuplicateAnswer();
    std::vector<Flock> m_flocks;
};

#endif // FlockManager_h
