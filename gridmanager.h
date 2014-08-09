#ifndef GridManager_h
#define GridManager_h

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

    void addPointToGrid(GPSPoint*);
    void dump();

private:
    double m_gridSize;
    std::unordered_map<std::string, Grid*> m_grids;
};

#endif // GridManager_h
