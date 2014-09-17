#ifndef Disk_h
#define Disk_h

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

    Disk(double centerX, double centerY, unsigned long timestamp)
        : m_centerX(centerX)
        , m_centerY(centerY)
        , m_timestamp(timestamp)
    { }

    double centerX() const { return m_centerX; }
    void setCenterX(double centerX) { m_centerX = centerX; }

    double centerY() const { return m_centerY; }
    void setCenterY(double centerY) { m_centerY = centerY; }

    unsigned long timestamp() const { return m_timestamp; }
    void addTrajectory(const Trajectory&);
    void addAlreadyComputedGrids(const std::vector<Grid*>&);
    void addAlreadyComputedGrid(Grid*);
    bool isGridAlreadyComputed(Grid*);
    unsigned numberOfTrajectories() const { return m_trajectories.size(); }
    unsigned countIntersection(Disk* other) const;

private:
    double m_centerX;
    double m_centerY;
    unsigned long m_timestamp;
    std::map<unsigned long, Trajectory> m_trajectories;
    std::unordered_set<Grid*> m_alreadyComputed;
};

#endif // Disk_h
