#include "trajectorymanager.h"

#include "trajectory.h"

void TrajectoryManager::addTrajectory(Trajectory* trajectory)
{
    std::unordered_map<long, Trajectory*>::const_iterator iter = m_trajectories.find(trajectory->id());
    if (iter == m_trajectories.end())
        m_trajectories[trajectory->id()] = trajectory;
    else
        iter->second->mergePoints(*trajectory);
}

Trajectory* TrajectoryManager::trajectoryById(long id)
{
    Trajectory* result = nullptr;
    std::unordered_map<long, Trajectory*>::const_iterator iter = m_trajectories.find(id);
    if (iter != m_trajectories.end())
        result = iter->second;

    return result;
}