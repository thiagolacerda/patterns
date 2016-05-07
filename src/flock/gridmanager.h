#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class GPSPoint;

#if !defined(NEWDESIGN)
class Grid;
#endif

class GridManager {
public:
    GridManager()
        : m_gridSize(0)
    { }

    explicit GridManager(double gridSize)
        : m_gridSize(gridSize)
    { }

    void addPointToGrid(const std::shared_ptr<GPSPoint>& point);
#if defined(NEWDESIGN)
    const std::unordered_map<std::string, std::vector<std::shared_ptr<GPSPoint>>> grids() { return m_grids; }
    std::vector<std::shared_ptr<GPSPoint>> extendedGrid(const std::string& key) const;
    double gridSize() const { return m_gridSize; }
    void setGridSize(double gridSize) { m_gridSize = gridSize; }
#else
    void dump();
    const std::unordered_map<std::string, Grid*> grids() { return m_grids; }
    void extendedGridPoints(const std::string& key, std::vector<std::shared_ptr<GPSPoint>>* extendedPoints);
#endif
    void clear();

private:
    double m_gridSize;
#if defined(NEWDESIGN)
    std::unordered_map<std::string, std::vector<std::shared_ptr<GPSPoint>>> m_grids;
#else
    std::unordered_map<std::string, Grid*> m_grids;
#endif
};

