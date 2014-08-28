#ifndef GridManager_h
#define GridManager_h

#include <memory>
#include <unordered_map>

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

private:
    double m_gridSize;
    std::unordered_map<std::string, std::shared_ptr<Grid>> m_grids;
};

#endif // GridManager_h
