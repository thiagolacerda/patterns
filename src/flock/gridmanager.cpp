#include "gridmanager.h"

#include <cmath>
#if defined(NEWDESIGN)
#include <cstdlib>
#else
#include <algorithm>
#include <iostream>
#include <sstream>
#include "grid.h"
#endif
#include "gpspoint.h"
#include "utils.h"

/*
 * Here we build the grid and add points to it.
 * We divide the space in cells, first one starting at 0. Get the point X coordinate and calculate in wich cell it fits
 * in.
 * Let's say, for instance, that the cell size is 20, so the grid would look like this:
 *
 *      0 - 19    20 - 29    30 - 39    40 - 49    50 - 59
 *     --------- ---------- ---------- ---------- ----------
 *    |         |          |          |          |          |
 *    |         |          |          |          |          | 0 - 19
 *    |         |          |          |          |          |
 *     --------- ---------- ---------- ---------- ----------
 *    |         |          |          |          |          |
 *    |         |          |          |          |          | 20 - 29
 *    |         |          |          |          |          |
 *     --------- ---------- ---------- ---------- ----------
 *    |         |          |          |          |          |
 *    |         |          |          |          |          | 30 - 39
 *    |         |          |          |          |          |
 *     --------- ---------- ---------- ---------- ----------
 *    |         |          |          |          |          |
 *    |         |          |          |          |          | 40 - 49
 *    |         |          |          |          |          |
 *     --------- ---------- ---------- ---------- ----------
 *
 * Point (32, 11) would fit in cell 0,2
 * We only allocate cell space if there is at least one point for that cell
 */
void GridManager::addPointToGrid(const std::shared_ptr<GPSPoint>& point)
{
    const double latMeters = point->latitudeMeters();
    const double longMeters = point->longitudeMeters();
    int xIndex = floor((Utils::distance(longMeters, latMeters, 0, latMeters) / m_gridSize)) + 1;
    int yIndex = floor((Utils::distance(longMeters, latMeters, longMeters, 0) / m_gridSize)) + 1;
    if (latMeters < 0)
        xIndex = -xIndex;
    if (longMeters < 0)
        yIndex = -yIndex;

#if defined(NEWDESIGN)
    std::string key = std::to_string(xIndex) + "_" + std::to_string(yIndex);
    m_grids[key].push_back(point);
#else
    std::ostringstream oss;
    oss << xIndex << "_" << yIndex;
    std::string key = oss.str();
    Grid* grid;
    auto iter = m_grids.find(key);
    if (iter == m_grids.end()) {
        grid = new Grid();
        m_grids[key] = grid;
    } else {
        grid = iter->second;
    }

    grid->addPoint(point);
#endif
}

void GridManager::clear()
{
#if !defined(NEWDESIGN)
    std::for_each(m_grids.begin(), m_grids.end(),
        [](const std::pair<std::string, Grid*>& elem) { delete elem.second; });
#endif
    m_grids.clear();
}

/*
 * The algorithm says that when we choose a cell the points that are eligible to try to build a disk with are only those
 * in the side by side neighbor cells, those on the diagonal and the cell itself.
 * We are only interested in creating disks when two points are far from each other at least the cell size.
 * So, for the exmple grid documented in addPoint method, if we are analysing cell 2,2, the eligible cells would be:
 * 1,1;1,2;1,3;2,1;2,2;2,3;3,1;3,2;3,3
 * In this method we return to the caller the vector of all eligible points.
 */
#if defined(NEWDESIGN)
std::vector<std::shared_ptr<GPSPoint>> GridManager::extendedGrid(const std::string& key) const
{
    std::vector<std::shared_ptr<GPSPoint>> extendedGrid;
    std::unordered_map<std::string, std::vector<std::shared_ptr<GPSPoint>>>::const_iterator iter;
    size_t underscoreIndex = key.find_first_of('_');
    int xIndex = atoi(key.substr(0, underscoreIndex).c_str());
    ++underscoreIndex;
    int yIndex = atoi(key.substr(underscoreIndex, key.length() - underscoreIndex).c_str());
    for (int i = xIndex - 1; i <= xIndex + 1; ++i) {
        for (int j = yIndex - 1; j <= yIndex + 1; ++j) {
            const auto key = std::to_string(i) + "_" + std::to_string(j);
            if ((iter = m_grids.find(key)) != m_grids.end())
                extendedGrid.insert(extendedGrid.end(), iter->second.begin(), iter->second.end());
        }
    }
    return extendedGrid;
}
#else
void GridManager::extendedGridPoints(const std::string& key, std::vector<std::shared_ptr<GPSPoint>>* extendedPoints)
{
    std::unordered_map<std::string, Grid*>::const_iterator iter;
    size_t underscoreIndex = key.find_first_of('_');
    int xIndex = atoi(key.substr(0, underscoreIndex).c_str());
    ++underscoreIndex;
    int yIndex = atoi(key.substr(underscoreIndex, key.length() - underscoreIndex).c_str());
    std::ostringstream oss;
    for (int i = xIndex - 1; i <= xIndex + 1; ++i) {
        for (int j = yIndex - 1; j <= yIndex + 1; ++j) {
            oss.str(std::string());
            oss << i << "_" << j;
            if ((iter = m_grids.find(oss.str())) != m_grids.end()) {
                const std::vector<std::shared_ptr<GPSPoint>>& gridPoints = iter->second->points();
                extendedPoints->insert(extendedPoints->end(), gridPoints.begin(), gridPoints.end());
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
#endif
