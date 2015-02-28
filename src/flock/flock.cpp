#include "flock.h"

#include <iostream>
#include "config.h"

void Flock::setTrajectories(const std::map<uint32_t, Trajectory>& trajectories)
{
    m_trajectories = trajectories;
}

/*
 * New trajectories for the next time slot are available for this flock, so we need to merge than with the old ones
 */
void Flock::mergeTrajectories(const std::map<uint32_t, Trajectory>& trajectories)
{
    for (auto iter = m_trajectories.begin(); iter != m_trajectories.end(); ++iter) {
        auto found = trajectories.find(iter->first);
        if (found != trajectories.end())
            iter->second.mergePoints(found->second);
    }
}

/*
 * This method is used this flock is a valid one for the current start and end time (endTime - startTime >= minLength).
 * So we now will start to look for a new flock starting on the next time slot, that'w why we are removing the points
 * for the first time slot of this flock.
 */
void Flock::clearFirstPoints()
{
    for (auto iter = m_trajectories.begin(); iter != m_trajectories.end(); ++iter)
        iter->second.clearFirstPoints();
}

void Flock::dump() const
{
    std::cout << "Flock's startTime: " << m_startTime << ", endTime: " << m_endTime << ", number of trajectories: "
        << m_trajectories.size() << std::endl;
}

void Flock::dumpTrajectories() const
{
    if (m_trajectories.empty())
        return;

    auto iter = m_trajectories.begin();
    std::cout << iter->second.id();
    ++iter;
    for (; iter != m_trajectories.end(); ++iter)
        std::cout << ", " << iter->second.id();
    std::cout << std::endl;
}
