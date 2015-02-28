#ifndef GRIDMANAGER_H
#define GRIDMANAGER_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class GPSPoint;
class Grid;

class GridManager {
public:
    GridManager()
        : m_gridSize(0)
    { }

    explicit GridManager(double gridSize)
        : m_gridSize(gridSize)
    { }

    void addPointToGrid(const std::shared_ptr<GPSPoint>&);
    void dump();
    const std::unordered_map<std::string, Grid*> grids() { return m_grids; }
    void extendedGridPoints(const std::string& key, std::vector<std::shared_ptr<GPSPoint>>* extendedPoints);
    void clear();

private:
    double m_gridSize;
    std::unordered_map<std::string, Grid*> m_grids;
};

#endif  // GRIDMANAGER_H
