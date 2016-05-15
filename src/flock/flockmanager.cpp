#include "flockmanager.h"

#if !defined(NEWDESIGN)
#include "config.h"
#endif
#include "disk.h"
#include "manager.h"

/*
 * Computes the intersection between two set of trajectories.
 * By default we return the number of common trajectories between those two sets.
 * If the parameter 'inter' is a valid map, we also store the common trajectories on that map
 */
uint32_t FlockManager::intersection(const Flock& flock, const std::shared_ptr<Disk>& disk,
    std::map<uint32_t, Trajectory>* inter)
{
    const auto& flockTrajs = flock.trajectories();
    const auto& diskPoints = disk->points();
    auto begin1 = flockTrajs.begin();
    auto begin2 = diskPoints.begin();
    auto end1 = flockTrajs.end();
    auto end2 = diskPoints.end();
    --end1;
    --end2;
    // The trajectory map is ordered by the trajectory's id, so if set1 has its first element less than set2's first
    // and its last element less than set2's last element, there is no intersection at all.
    if ((begin1->first < begin2->first && end1->first < begin2->first) ||
        (begin2->first < begin1->first && end2->first < begin1->first))
        return 0;

    uint32_t count = 0;
    while (begin1 != flockTrajs.end() && begin2 != diskPoints.end()) {
        if (begin1->first < begin2->first) {
            ++begin1;
        } else if (begin2->first < begin1->first) {
            ++begin2;
        } else {
            if (inter) {
                Trajectory t = begin1->second;
                t.addPoint(begin2->second);
                (*inter).emplace(std::make_pair(begin1->first, std::move(t)));
            }
            ++count;
            ++begin1;
            ++begin2;
        }
    }
    return count;
}

uint32_t FlockManager::intersection(const Flock& flock1, const Flock& flock2)
{
    const auto& flock1Trajs = flock1.trajectories();
    const auto& flock2Trajs = flock2.trajectories();
    auto begin1 = flock1Trajs.begin();
    auto begin2 = flock2Trajs.begin();
    auto end1 = flock1Trajs.end();
    auto end2 = flock2Trajs.end();
    --end1;
    --end2;
    // The trajectory map is ordered by the trajectory's id, so if set1 has its first element less than set2's first
    // and its last element less than set2's last element, there is no intersection at all.
    if ((begin1->first < begin2->first && end1->first < begin2->first) ||
        (begin2->first < begin1->first && end2->first < begin1->first))
        return 0;

    uint32_t count = 0;
    while (begin1 != flock1Trajs.end() && begin2 != flock2Trajs.end()) {
        if (begin1->first < begin2->first) {
            ++begin1;
        } else if (begin2->first < begin1->first) {
            ++begin2;
        } else {
            ++count;
            ++begin1;
            ++begin2;
        }
    }
    return count;
}

#if defined(NEWDESIGN)
void FlockManager::tryMergeFlocks(const std::vector<std::shared_ptr<Disk>>& disks, uint64_t timestamp)
#else
void FlockManager::tryMergeFlocks(const std::vector<std::shared_ptr<Disk>>& disks)
#endif
{
    bool hasFlocks = !m_flocks.empty();
    std::vector<Flock> flocksFromDisks;
    if (hasFlocks) {
        // There are some flocks from previuos time instance, so we try to merge the computed disks with them
        for (auto diskIter = disks.begin(); diskIter != disks.end(); ++diskIter) {
            std::shared_ptr<Disk> disk = *diskIter;
            for (auto existingFlock = m_flocks.begin(); existingFlock != m_flocks.end(); ++existingFlock) {
                if (disk->timestamp() - (*existingFlock).endTime() > 1)
                    continue;

                std::map<uint32_t, Trajectory> inter;
                intersection(*existingFlock, disk, &inter);
#if defined(NEWDESIGN)
                if (inter.size() >= m_trajectoriesPerFlock) {
#else
                if (inter.size() >= Config::numberOfTrajectoriesPerFlock()) {
#endif
                    // We have a pontential increment for a previous flock
                    Flock newFlock;
                    newFlock.setTrajectories(inter);
                    newFlock.setStartTime((*existingFlock).startTime());
                    newFlock.setEndTime(disk->timestamp());
                    // Before inserting it in FlockManager we need to check if there isn't any other flock that is a
                    // superset of it
                    mergeFlocks(&flocksFromDisks, newFlock);
                }
            }
        }
    }

    for (auto diskIter = disks.begin(); diskIter != disks.end(); ++diskIter) {
        Flock newFlock;
        newFlock.setTrajectoriesFromPoints((*diskIter)->points());
        newFlock.setStartTime((*diskIter)->timestamp());
        newFlock.setEndTime((*diskIter)->timestamp());
        if (!hasFlocks) {
            m_flocks.push_back(newFlock);
            updateBuffers(newFlock);
        } else {
            mergeFlocks(&flocksFromDisks, newFlock);
        }
    }
    if (hasFlocks)
        m_flocks = flocksFromDisks;

#if defined(NEWDESIGN)
    m_lastTimestamp = timestamp;
#endif
}

void FlockManager::updateBuffers(const Flock& flock)
{
#if !defined(NEWDESIGN)
    if (!Config::buffering())
        return;
#endif

    const auto& trajectories = flock.trajectories();
    for (const auto& trajPair : trajectories) {
        if (m_updated.find(trajPair.first) != m_updated.end())
           continue;

#if defined(NEWDESIGN)
        updatePresenceMap(trajPair.first);
#else
        m_manager->addSequenceEntry(trajPair.first);
#endif
        m_updated.insert(trajPair.first);
    }
}

/*
 * Check if this flock can be inserted (it can't be a subset of an already existing flock)
 */
void FlockManager::mergeFlocks(std::vector<Flock>* flocks, const Flock& newFlock)
{
    for (auto existingFlock = flocks->begin(); existingFlock != flocks->end();) {
        uint32_t count = intersection(*existingFlock, newFlock);
        if (newFlock.trajectories().size() == count) {
            // This new flock is, potentially, a subset of (*existingFlock)
            if ((*existingFlock).startTime() <= newFlock.startTime())
                // (*existingFlock) start time is less than new flock's start time so new flock is definitely a subset of (*existingFlock)
                return;

            if ((*existingFlock).trajectories().size() > count)
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
    updateBuffers(newFlock);
}

/*
 * See if any of the flocks stored in FlockManager if a valid Flock and return them to the caller
 */
std::vector<Flock> FlockManager::reportFlocks()
{
    std::vector<Flock> results;
#if !defined(NEWDESIGN)
    uint32_t flockLength = Config::flockLength();
#endif
    for (auto flock = m_flocks.begin(); flock != m_flocks.end();) {
#if defined(NEWDESIGN)
        if ((*flock).endTime() - (*flock).startTime() + 1 >= m_flockLength) {
#else
        if ((*flock).endTime() - (*flock).startTime() + 1 >= flockLength) {
#endif
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

    m_updated.clear();
#if defined(NEWDESIGN)
    ++m_buffered;
    if (m_buffered >= m_flockLength) {
        --m_buffered;
        shiftPresenceMaps();
    }
#endif
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
                interCount = intersection(flock, *it2);
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

#if defined(NEWDESIGN)
void FlockManager::updatePresenceMap(uint32_t trajectoryId)
{
    m_presenceMap[trajectoryId] |= (1 << m_buffered);
}

void FlockManager::shiftPresenceMaps()
{
    for (auto iter = m_presenceMap.begin(); iter != m_presenceMap.end();) {
        if (iter->second) {
            iter->second >>= 1;
            ++iter;
        } else {
            iter = m_presenceMap.erase(iter);
        }
    }
}

void FlockManager::preProcessing(uint64_t timestamp)
{
    if (timestamp - m_lastTimestamp > 1)
        clear();
}

void FlockManager::clear()
{
    m_flocks.clear();
    m_presenceMap.clear();
    m_updated.clear();
    m_buffered = 0;
}
#else
void FlockManager::dump() const
{
    for (const Flock& flock : m_flocks) {
        flock.dump();
        flock.dumpTrajectories();
    }
}
#endif
