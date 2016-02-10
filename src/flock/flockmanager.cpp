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
    bool hasFlocks = !m_flocks.empty();
    std::vector<Flock> flocksFromDisks;
    if (hasFlocks) {
        // There are some flocks from previuos time instance, so we try to merge the computed disks with them
        for (auto diskIter = disks.begin(); diskIter != disks.end(); ++diskIter) {
            Disk* disk = *diskIter;
            for (auto existingFlock = m_flocks.begin(); existingFlock != m_flocks.end(); ++existingFlock) {
                if (disk->timestamp() - (*existingFlock).endTime() > 1)
                    continue;

                std::map<uint32_t, Trajectory> inter;
                intersection((*existingFlock).trajectories(), disk->trajectories(), &inter);
                if (inter.size() >= Config::numberOfTrajectoriesPerFlock()) {
                    // We have a pontential increment for a previous flock
                    Flock newFlock;
                    newFlock.setTrajectories(inter);
                    newFlock.setStartTime((*existingFlock).startTime());
                    newFlock.setEndTime(disk->timestamp());
                    // Before inserting it in FlockManager we need to check if there isn't any other flock that is a
                    // superset of it
                    if (mergeFlocks(&flocksFromDisks, newFlock))
                        flocksFromDisks.back().mergeTrajectories(disk->trajectories());
                }
            }
        }
    }

    for (auto diskIter = disks.begin(); diskIter != disks.end(); ++diskIter) {
        Flock newFlock;
        newFlock.setTrajectories((*diskIter)->trajectories());
        newFlock.setStartTime((*diskIter)->timestamp());
        newFlock.setEndTime((*diskIter)->timestamp());
        if (!hasFlocks)
            m_flocks.push_back(newFlock);
        else
            mergeFlocks(&flocksFromDisks, newFlock);
    }
    if (hasFlocks)
        m_flocks = flocksFromDisks;
}

/*
 * Check if this flock can be inserted (it can't be a subset of an already existing flock)
 */
bool FlockManager::mergeFlocks(std::vector<Flock>* flocks, const Flock& newFlock)
{
    for (auto existingFlock = flocks->begin(); existingFlock != flocks->end();) {
        uint32_t count = intersection((*existingFlock).trajectories(), newFlock.trajectories(), nullptr);
        if (newFlock.trajectories().size() == count) {
            // This new flock is, potentially, a subset of (*existingFlock)
            if ((*existingFlock).startTime() <= newFlock.startTime())
                // (*existingFlock) start time is less than new flock's start time so new flock is definitely a subset of (*existingFlock)
                return false;
            else if ((*existingFlock).trajectories().size() > count)
                // (*existingFlock) and new flock do not have the same trajectories so we need to keep both
                ++existingFlock;
            else
                // (*existingFlock) is a subset of new flock: They have the same number of trajectories and new flock's
                // start time is not earlier than (*existingFlock)
                existingFlock = flocks->erase(existingFlock);
        } else if ((*existingFlock).trajectories().size() == count) {
            // (*existingFlock) is, potentially, a subset of new flock
            if ((*existingFlock).startTime() < newFlock.startTime())
                // (*existingFlock) is older, so we keep both
                ++existingFlock;
            else
                existingFlock = flocks->erase(existingFlock);
        } else {
            ++existingFlock;
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
    for (auto flock = m_flocks.begin(); flock != m_flocks.end();) {
        if ((*flock).endTime() - (*flock).startTime() + 1 >= flockLength) {
            (*flock).assignId();
            results.push_back(*flock);
            // This is valid flock, so let's increment its start time to check for a new flock starting in the
            // subsequent time slot
            (*flock).setStartTime((*flock).startTime() + 1);
            if ((*flock).startTime() > (*flock).endTime()) {
                // Empty flock
                flock = m_flocks.erase(flock);
                continue;
            }
            (*flock).assignId();
            // Remove the points that belong to the first time slot of this flock
            (*flock).clearFirstPoints();
        }
        ++flock;
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
