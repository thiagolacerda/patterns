#include "gridmanager.h"

#include <iostream>
#include <sstream>
#include <string>
#include "gpspoint.h"
#include "grid.h"
#include "utils.h"

void GridManager::addPointToGrid(const std::shared_ptr<GPSPoint>& point)
{
    double latitude = point->latitude();
    double longitude = point->longitude();
    int xIndex = (Utils::distance(latitude, longitude, 0, longitude) / m_gridSize) + 1;
    int yIndex = (Utils::distance(latitude, longitude, latitude, 0) / m_gridSize) + 1;
    if (latitude < 0)
        xIndex = -xIndex;
    if (longitude < 0)
        yIndex = -yIndex;

    std::ostringstream oss;
    oss << xIndex << "_" << yIndex;
    std::string key = oss.str();
    std::shared_ptr<Grid> grid;
    auto iter = m_grids.find(key);
    if (iter == m_grids.end()) {
        grid.reset(new Grid);
        m_grids[key] = grid;
    } else
        grid = iter->second;

    grid->addPoint(point);
}

void GridManager::dump()
{
    std::cout << "Number of grids: " << m_grids.size() << std::endl;
    for (auto iter = m_grids.begin(); iter != m_grids.end(); ++iter) {
        std::cout << "Grid: " << iter->first << std::endl << "\t";
        iter->second->dump();
    }
}
