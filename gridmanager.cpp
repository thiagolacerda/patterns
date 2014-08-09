#include "gridmanager.h"

#include <iostream>
#include <sstream>
#include <string>
#include "gpspoint.h"
#include "grid.h"

void GridManager::addPointToGrid(GPSPoint* point)
{
    int xIndex = (point->distanceToPoint(GPSPoint(0, point->longitude(), 0, nullptr)) / m_gridSize) + 1;
    int yIndex = (point->distanceToPoint(GPSPoint(point->latitude(), 0, 0, nullptr)) / m_gridSize) + 1;
    if (point->latitude() < 0)
        xIndex = -xIndex;
    if (point->longitude() < 0)
        yIndex = -yIndex;

    std::ostringstream oss;
    oss << xIndex << "_" << yIndex;
    std::string key = oss.str();
    Grid* grid = nullptr;
    std::unordered_map<std::string, Grid*>::const_iterator iter = m_grids.find(key);
    if (iter == m_grids.end()) {
        grid = new Grid();
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
