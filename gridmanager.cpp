#include "gridmanager.h"

#include <algorithm>
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
    Grid* grid;
    auto iter = m_grids.find(key);
    if (iter == m_grids.end()) {
        grid = new Grid();
        m_grids[key] = grid;
    } else
        grid = iter->second;

    grid->addPoint(point);
}

void GridManager::clear()
{
    std::for_each(m_grids.begin(), m_grids.end(), [](const std::pair<std::string, Grid*>& elem) { delete elem.second; });
    m_grids.clear();
}

void GridManager::neighborsGridPoints(const std::string& key, std::vector<std::shared_ptr<GPSPoint>>& points)
{
    std::unordered_map<std::string, Grid*>::const_iterator iter;
    size_t underscoreIndex = key.find_first_of('_');
    int xIndex = atoi(key.substr(0, underscoreIndex).c_str());
    ++underscoreIndex;
    int yIndex = atoi(key.substr(underscoreIndex, key.length() - underscoreIndex).c_str());
    std::ostringstream oss;
    for (int i = xIndex - 1; i <= xIndex + 1; ++i) {
        for (int j = yIndex - 1; j <= yIndex + 1; ++j) {
            if (i == xIndex && j == yIndex)
                continue;

            oss.str(std::string());
            oss << i << "_" << j;
            if ((iter = m_grids.find(oss.str())) != m_grids.end()) {
                const std::vector<std::shared_ptr<GPSPoint>>& gridPoints = iter->second->points();
                points.insert(points.end(), gridPoints.begin(), gridPoints.end());
            }
        }
    }
}

void GridManager::dump()
{
    std::cout << "Number of grids: " << m_grids.size() << std::endl;
    for (auto iter = m_grids.begin(); iter != m_grids.end(); ++iter) {
        std::cout << "Grid: " << iter->first << std::endl << "\t";
        iter->second->dump();
    }
}
