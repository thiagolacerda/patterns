#include "disk.h"

#include <algorithm>
#include "grid.h"

void Disk::addTrajectory(const std::shared_ptr<Trajectory>& trajectory)
{
    m_trajectories.insert(trajectory);
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
