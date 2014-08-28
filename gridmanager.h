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
    const std::unordered_map<std::string, std::shared_ptr<Grid>> grids() { return m_grids; }
    void neighborGridsAndPoints(const std::string& key, std::vector<std::shared_ptr<Grid>>& grids,
        std::vector<std::shared_ptr<GPSPoint>>& points);
    std::shared_ptr<Grid> gridThatPointBelongsTo(const std::shared_ptr<GPSPoint>& point);
    void clear();

private:
    double m_gridSize;
    std::unordered_map<std::string, std::shared_ptr<Grid>> m_grids;
    // This is a bit redundant but will allow fast lookup, when queying to which grid a point belongs to.
    std::unordered_map<std::shared_ptr<GPSPoint>, std::shared_ptr<Grid>> m_pointsPerGrid;
};

#endif // GridManager_h
