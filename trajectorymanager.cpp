#include "trajectorymanager.h"

#include <iostream>
#include "trajectory.h"

void TrajectoryManager::addTrajectory(Trajectory* trajectory)
{
    auto iter = m_trajectories.find(trajectory->id());
    if (iter == m_trajectories.end())
        m_trajectories[trajectory->id()] = trajectory;
    else
        iter->second->mergePoints(*trajectory);
}

Trajectory* TrajectoryManager::trajectoryById(unsigned long id)
{
    auto iter = m_trajectories.find(id);
    if (iter != m_trajectories.end())
        return iter->second;

    return nullptr;
}

void TrajectoryManager::dump()
{
    std::cout << "Number of trajectories: " << m_trajectories.size() << std::endl;
    for (auto iter = m_trajectories.begin(); iter != m_trajectories.end(); ++iter) {
        std::cout << "trajectory: " << iter->first << std::endl;
        iter->second->dump();
    }
}
