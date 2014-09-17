#include "flock.h"

#include "config.h"
#include <iostream>

void Flock::setTrajectories(const std::map<unsigned long, Trajectory>& trajectories)
{
    m_trajectories = trajectories;
}

void Flock::mergeTrajectories(const std::map<unsigned long, Trajectory>& trajectories)
{
    for (auto iter = m_trajectories.begin(); iter != m_trajectories.end(); ++iter) {
        auto found = trajectories.find(iter->first);
        if (found != trajectories.end())
            iter->second.mergePoints(found->second);
    }
}

void Flock::clearFirstPoints()
{
    for (auto iter = m_trajectories.begin(); iter != m_trajectories.end(); ++iter)
        iter->second.clearFirstPoints();
}

void Flock::dump() const
{
    std::cout << "Flock's startTime: " << m_startTime << ", endTime: " << m_endTime << ", number of trajectories: " << m_trajectories.size() << std::endl;
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
