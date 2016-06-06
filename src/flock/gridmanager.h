#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class GPSPoint;

class GridManager {
public:
    GridManager()
        : m_gridSize(0)
    { }

    explicit GridManager(double gridSize)
        : m_gridSize(gridSize)
    { }

    void addPointToGrid(const std::shared_ptr<GPSPoint>& point);
    const std::unordered_map<std::string, std::vector<std::shared_ptr<GPSPoint>>> grids() { return m_grids; }
    std::vector<std::shared_ptr<GPSPoint>> extendedGrid(const std::string& key) const;
    double gridSize() const { return m_gridSize; }
    void setGridSize(double gridSize) { m_gridSize = gridSize; }
    void clear();

private:
    double m_gridSize;
    std::unordered_map<std::string, std::vector<std::shared_ptr<GPSPoint>>> m_grids;
};

