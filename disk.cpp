#include "disk.h"

#include <algorithm>
#include "grid.h"

void Disk::addTrajectory(const std::shared_ptr<Trajectory>& trajectory)
{
    m_trajectories[trajectory->id()] = trajectory;
}

void Disk::addAlreadyComputedGrids(const std::vector<std::shared_ptr<Grid>>& grids)
{
    std::copy(grids.begin(), grids.end(), std::inserter(m_alreadyComputed, m_alreadyComputed.end()));
}

void Disk::addAlreadyComputedGrid(const std::shared_ptr<Grid>& grid)
{
    m_alreadyComputed.insert(grid);
}

bool Disk::isGridAlreadyComputed(const std::shared_ptr<Grid>& grid)
{
    return m_alreadyComputed.find(grid) != m_alreadyComputed.end();
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
        if (begin1->first < begin2->first)
            ++begin1;
        else if (begin2->first < begin1->first)
            ++begin2;
        else {
            ++count;
            ++begin1;
            ++begin2;
        }
    }

    return count;
}