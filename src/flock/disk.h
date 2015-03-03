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
    Disk();
    Disk(double centerX, double centerY, uint32_t timestamp);

    double centerX() const { return m_centerX; }
    double centerLongitude() const { return m_centerLongitude; }

    double centerY() const { return m_centerY; }
    double centerLatitude() const { return m_centerLatitude; }

    uint32_t timestamp() const { return m_timestamp; }
    void addTrajectory(const Trajectory&);
    unsigned numberOfTrajectories() const { return m_trajectories.size(); }
    unsigned countIntersection(Disk* other) const;
    const std::map<uint32_t, Trajectory>& trajectories() const { return m_trajectories; }
    void dumpTrajectories() const;
    void dump() const;

private:
    double m_centerX;
    double m_centerLongitude;
    double m_centerY;
    double m_centerLatitude;
    uint32_t m_timestamp;
    std::map<uint32_t, Trajectory> m_trajectories;
};

#endif  // DISK_H
