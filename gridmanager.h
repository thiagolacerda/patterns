#ifndef GridManager_h
#define GridManager_h

#include <memory>
#include <unordered_map>
#include <vector>

class GPSPoint;
class Grid;

class GridManager {
public:
    GridManager()
        : m_gridSize(0)
    { }

    GridManager(double gridSize)
        : m_gridSize(gridSize)
    { }

    void addPointToGrid(const std::shared_ptr<GPSPoint>&);
    void dump();
    const std::unordered_map<std::string, Grid*> grids() { return m_grids; }
    void neighborsGridPoints(const std::string& key, std::vector<std::shared_ptr<GPSPoint>>& points);
    void clear();

private:
    double m_gridSize;
    std::unordered_map<std::string, Grid*> m_grids;
};

#endif // GridManager_h
