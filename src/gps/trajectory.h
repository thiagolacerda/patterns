#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <cstdint>
#include <memory>
#include <vector>

class GPSPoint;

class Trajectory {
public:
    Trajectory() { }

    explicit Trajectory(uint32_t id)
        : m_id(id)
    { }

    void addPoint(const std::shared_ptr<GPSPoint>&);
    const std::vector<std::shared_ptr<GPSPoint>>& points() const { return m_points; }
    uint32_t id() const { return m_id; }
    void mergePoints(const Trajectory&);
    void clearFirstPoints();
    void dump();

private:
    uint32_t m_id;
    std::vector<std::shared_ptr<GPSPoint>> m_points;
};

#endif  // TRAJECTORY_H
