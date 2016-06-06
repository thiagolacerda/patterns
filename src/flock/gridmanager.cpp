#include "gridmanager.h"

#include <cmath>
#include <cstdlib>
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

    std::string key = std::to_string(xIndex) + "_" + std::to_string(yIndex);
    m_grids[key].push_back(point);
}

void GridManager::clear()
{
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

