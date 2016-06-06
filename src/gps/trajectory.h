#pragma once

#include <memory>
#include <vector>

class GPSPoint;

class Trajectory {
public:
    Trajectory() { }

    explicit Trajectory(uint32_t id)
        : m_id(id)
    { }

    explicit Trajectory(uint32_t id, const std::shared_ptr<GPSPoint>& point)
        : m_id(id)
    {
        addPoint(point);
    }

    void addPoint(const std::shared_ptr<GPSPoint>& point);
    const std::vector<std::shared_ptr<GPSPoint>>& points() const { return m_points; }
    uint32_t id() const { return m_id; }
    void mergePoints(const Trajectory&);
    void clearFirstPoints();

private:
    uint32_t m_id;
    std::vector<std::shared_ptr<GPSPoint>> m_points;
};

