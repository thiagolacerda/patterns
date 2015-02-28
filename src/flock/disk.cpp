#include "disk.h"

#include <algorithm>
#include <iostream>
#include "grid.h"

void Disk::addTrajectory(const Trajectory& trajectory)
{
    m_trajectories[trajectory.id()] = trajectory;
}

unsigned Disk::countIntersection(Disk* other) const
{
    auto begin1 = m_trajectories.begin();
    auto begin2 = other->m_trajectories.begin();
    auto end1 = m_trajectories.end();
    auto end2 = other->m_trajectories.end();
    --end1;
    --end2;
    if ((begin1->first < begin2->first && end1->first < begin2->first) ||
        (begin2->first < begin1->first && end2->first < begin1->first))
        return 0;

    unsigned count = 0;
    while (begin1 != m_trajectories.end() && begin2 != other->m_trajectories.end()) {
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

void Disk::dumpTrajectories() const
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

void Disk::dump() const
{
    std::cout << "Disk, x: " << m_centerX << ", y: " << m_centerY << " trajectories: "
        << m_trajectories.size() << std::endl;
}
