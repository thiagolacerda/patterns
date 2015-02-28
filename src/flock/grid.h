#ifndef GRID_H
#define GRID_H

#include <memory>
#include <vector>

class GPSPoint;

class Grid {
public:
    Grid() {}

    void addPoint(const std::shared_ptr<GPSPoint>&);
    const std::vector<std::shared_ptr<GPSPoint>>& points() { return m_points; }
    void dump();

private:
    std::vector<std::shared_ptr<GPSPoint>> m_points;
};

#endif  // GRID_H
