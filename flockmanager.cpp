#include "flockmanager.h"

#include "config.h"
#include "disk.h"

unsigned FlockManager::intersection(const std::map<unsigned long, Trajectory>& set1,
    const std::map<unsigned long, Trajectory>& set2, std::map<unsigned long, Trajectory>* inter)
{
    auto begin1 = set1.begin();
    auto begin2 = set2.begin();
    auto end1 = set1.end();
    auto end2 = set2.end();
    end1--;
    end2--;
    if ((begin1->first < begin2->first && end1->first < begin2->first) ||
        (begin2->first < begin1->first && end2->first < begin1->first))
        return 0;

    unsigned count = 0;
    while (begin1 != set1.end() && begin2 != set2.end()) {
        if (begin1->first < begin2->first)
            ++begin1;
        else if (begin2->first < begin1->first)
            ++begin2;
        else {
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
        for (auto it1 = disks.begin(); it1 != disks.end(); ++it1) {
            Disk* disk = *it1;
            for (auto it2 = m_flocks.begin(); it2 != m_flocks.end(); ++it2) {
                std::map<unsigned long, Trajectory> inter;
                intersection((*it2).trajectories(), disk->trajectories(), &inter);
                if (inter.size() >= Config::numberOfTrajectoriesPerFlock()) {
                    Flock newFlock;
                    newFlock.setTrajectories(inter);
                    newFlock.setStartTime((*it2).startTime());
                    newFlock.setEndTime(disk->timestamp() + Config::timeSlotSize());
                    if (mergeFlocks(flocksFromDisks, newFlock))
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
            mergeFlocks(flocksFromDisks, newFlock);
    }
    if (!noFlocksInitialy)
        m_flocks = flocksFromDisks;
}

bool FlockManager::mergeFlocks(std::vector<Flock>& flocks, const Flock& newFlock)
{
    for (auto it1 = flocks.begin(); it1 != flocks.end();) {
        unsigned count = intersection((*it1).trajectories(), newFlock.trajectories(), nullptr);
        if (newFlock.trajectories().size() == count) {
            if ((*it1).startTime() <= newFlock.startTime())
                return false;
            else if ((*it1).trajectories().size() > count)
                ++it1;
            else
                it1 = flocks.erase(it1);
        } else if ((*it1).trajectories().size() == count) {
            if ((*it1).startTime() < newFlock.startTime())
                ++it1;
            else
                it1 = flocks.erase(it1);
        } else
            ++it1;
    }
    flocks.push_back(newFlock);
    return true;
}

std::vector<Flock> FlockManager::reportFlocks()
{
    std::vector<Flock> results;
    unsigned long flockLength = Config::flockLength();
    for (auto iter = m_flocks.begin(); iter != m_flocks.end(); ++iter) {
        if (((*iter).endTime() - (*iter).startTime()) >= flockLength) {
            results.push_back(*iter);
            (*iter).setStartTime((*iter).startTime() + Config::timeSlotSize());
            (*iter).clearFirstPoints();
        }
    }
    if (!results.empty())
        checkDuplicateAnswer();

    return results;
}

void FlockManager::checkDuplicateAnswer()
{
    if (m_flocks.empty())
        return;

    std::vector<Flock> flocks;
    unsigned int interCount;
    bool insert;

    for (auto it1 = m_flocks.begin(); it1 != m_flocks.end(); ++it1) {
        insert = true;
        const Flock& flock = (*it1);
        for (auto it2 = flocks.begin(); insert && it2 != flocks.end();) {
            if (flock.startTime() == (*it2).startTime() && flock.endTime() == (*it2).endTime()) {
                interCount = intersection(flock.trajectories(), (*it2).trajectories(), nullptr);
                if (flock.trajectories().size() == interCount)
                    insert = false;
                else if ((*it1).trajectories().size() == interCount)
                    it2 = flocks.erase(it2);
                else
                    ++it2;
            } else
                ++it2;
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
