#include "flockmanager.h"

#include "config.h"
#include "disk.h"

/*
 * Computes the intersection between two set of trajectories.
 * By default we return the number of common trajectories between those two sets.
 * If the parameter 'inter' is a valid map, we also store the common trajectories on that map
 */
uint32_t FlockManager::intersection(const std::map<uint32_t, Trajectory>& set1,
    const std::map<uint32_t, Trajectory>& set2, std::map<uint32_t, Trajectory>* inter)
{
    auto begin1 = set1.begin();
    auto begin2 = set2.begin();
    auto end1 = set1.end();
    auto end2 = set2.end();
    end1--;
    end2--;
    // The trajectory map is ordered by the trajectory's id, so if set1 has its first element less than set2's first
    // and its last element less than set2's last element, there is no intersection at all.
    if ((begin1->first < begin2->first && end1->first < begin2->first) ||
        (begin2->first < begin1->first && end2->first < begin1->first))
        return 0;

    uint32_t count = 0;
    while (begin1 != set1.end() && begin2 != set2.end()) {
        if (begin1->first < begin2->first) {
            ++begin1;
        } else if (begin2->first < begin1->first) {
            ++begin2;
        } else {
            if (inter)
                (*inter)[begin1->first] = begin1->second;
            ++count;
            ++begin1;
            ++begin2;
        }
    }
    return count;
}

void FlockManager::tryMergeFlocks(const std::vector<Disk*>& disks)
{
    bool noFlocksInitialy = m_flocks.empty();
    std::vector<Flock> flocksFromDisks;
    if (!noFlocksInitialy) {
        // There are some flocks from previuos time instance, so we try to merge the computed disks with them
        for (auto it1 = disks.begin(); it1 != disks.end(); ++it1) {
            Disk* disk = *it1;
            for (auto it2 = m_flocks.begin(); it2 != m_flocks.end(); ++it2) {
                std::map<uint32_t, Trajectory> inter;
                intersection((*it2).trajectories(), disk->trajectories(), &inter);
                if (inter.size() >= Config::numberOfTrajectoriesPerFlock()) {
                    // We have a pontential increment for a previous flock
                    Flock newFlock;
                    newFlock.setTrajectories(inter);
                    newFlock.setStartTime((*it2).startTime());
                    newFlock.setEndTime(disk->timestamp() + Config::timeSlotSize());
                    // Before inserting it in FlockManager we need to check if there isn't any other flock that is a
                    // superset of it
                    if (mergeFlocks(&flocksFromDisks, newFlock))
                        newFlock.mergeTrajectories(disk->trajectories());
                }
            }
        }
    }

    for (auto iter = disks.begin(); iter != disks.end(); ++iter) {
        Flock newFlock;
        newFlock.setTrajectories((*iter)->trajectories());
        newFlock.setStartTime((*iter)->timestamp());
        newFlock.setEndTime((*iter)->timestamp() + Config::timeSlotSize());
        if (noFlocksInitialy)
            m_flocks.push_back(newFlock);
        else
            mergeFlocks(&flocksFromDisks, newFlock);
    }
    if (!noFlocksInitialy)
        m_flocks = flocksFromDisks;
}

/*
 * Check if this flock can be inserted (it can't be a subset of an already existing flock)
 */
bool FlockManager::mergeFlocks(std::vector<Flock>* flocks, const Flock& newFlock)
{
    for (auto it1 = flocks->begin(); it1 != flocks->end();) {
        uint32_t count = intersection((*it1).trajectories(), newFlock.trajectories(), nullptr);
        if (newFlock.trajectories().size() == count) {
            // This new flock is, potentially, a subset of (*it)
            if ((*it1).startTime() <= newFlock.startTime())
                // (*it) start time is less than new flock's start time so new flock is definitely a subset of (*it)
                return false;
            else if ((*it1).trajectories().size() > count)
                // (*it) and new flock do not have the same trajectories so we need to keep both
                ++it1;
            else
                // (*it1) is a subset of new flock: They have the same number of trajectories and new flock's start time
                // is not earlier than (*it1)
                it1 = flocks->erase(it1);
        } else if ((*it1).trajectories().size() == count) {
            // (*it1) is, potentially, a subset of new flock
            if ((*it1).startTime() < newFlock.startTime())
                // (*it1) is older, so we keep both
                ++it1;
            else
                it1 = flocks->erase(it1);
        } else {
            ++it1;
        }
    }
    flocks->push_back(newFlock);
    return true;
}

/*
 * See if any of the flocks stored in FlockManager if a valid Flock and return them to the caller
 */
std::vector<Flock> FlockManager::reportFlocks()
{
    std::vector<Flock> results;
    uint32_t flockLength = Config::flockLength();
    for (auto iter = m_flocks.begin(); iter != m_flocks.end();) {
        if (((*iter).endTime() - (*iter).startTime()) >= flockLength) {
            results.push_back(*iter);
            // This is valid flock, so let's increment its start time to check for a new flock starting in the
            // subsequent time slot
            (*iter).setStartTime((*iter).startTime() + Config::timeSlotSize());
            if ((*iter).startTime() == (*iter).endTime()) {
                // Empty flock
                iter = m_flocks.erase(iter);
                continue;
            }
            // Remove the points that belong to the first time slot of this flock
            (*iter).clearFirstPoints();
        }
        ++iter;
    }
    if (!results.empty())
        // When incrementing the the start time of flock, there could be another flock with the same trajectories and
        // the same start time and end time, so we need to clear that duplicated results
        checkDuplicateAnswer();

    return results;
}

/*
 * This method checks if there are flocks that are subset of others and remove them
 */
void FlockManager::checkDuplicateAnswer()
{
    if (m_flocks.empty())
        return;

    // This vector will contain only the unique flocks (no subsets)
    std::vector<Flock> flocks;
    uint32_t interCount;

    for (auto it1 = m_flocks.begin(); it1 != m_flocks.end(); ++it1) {
        bool insert = true;
        const Flock& flock = (*it1);
        for (auto it2 = flocks.begin(); insert && it2 != flocks.end();) {
            if (flock.startTime() == (*it2).startTime() && flock.endTime() == (*it2).endTime()) {
                // Same start time and end time, let's look if they have the same trajectories
                interCount = intersection(flock.trajectories(), (*it2).trajectories(), nullptr);
                if (flock.trajectories().size() == interCount)
                    // flock is a subset of (*it2)
                    insert = false;
                else if ((*it1).trajectories().size() == interCount)
                    // (*it2) is a subset of flock
                    it2 = flocks.erase(it2);
                else
                    ++it2;
            } else {
                ++it2;
            }
        }
        if (insert)
            flocks.push_back(flock);
    }

    m_flocks = flocks;
}

void FlockManager::dump() const
{
    for (const Flock& flock : m_flocks) {
        flock.dump();
        flock.dumpTrajectories();
    }
}
