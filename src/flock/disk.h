#ifndef DISK_H
#define DISK_H

#include <cstdint>
#include <map>
#include <memory>
#include <unordered_set>
#include <vector>
#include "trajectory.h"

class Grid;

class Disk {
public:
    Disk()
        : m_centerX(0)
        , m_centerY(0)
        , m_timestamp(0)
    { }

    Disk(double centerX, double centerY, uint32_t timestamp)
        : m_centerX(centerX)
        , m_centerY(centerY)
        , m_timestamp(timestamp)
    { }

    double centerX() const { return m_centerX; }

    double centerY() const { return m_centerY; }

    uint32_t timestamp() const { return m_timestamp; }
    void addTrajectory(const Trajectory&);
    unsigned numberOfTrajectories() const { return m_trajectories.size(); }
    unsigned countIntersection(Disk* other) const;
    const std::map<uint32_t, Trajectory>& trajectories() const { return m_trajectories; }
    void dumpTrajectories() const;
    void dump() const;

private:
    double m_centerX;
    double m_centerY;
    uint32_t m_timestamp;
    std::map<uint32_t, Trajectory> m_trajectories;
};

#endif  // DISK_H
