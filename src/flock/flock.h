#ifndef FLOCK_H
#define FLOCK_H

#include <cstdint>
#include <map>
#include "trajectory.h"

// A flock is composed of a set of trajectories (which are composed by a set of points), with a start and end time
class Flock {
public:
    Flock()
        : m_startTime(0)
        , m_endTime(0)
    { }

    Flock(uint32_t startTime, uint32_t endTime)
        : m_startTime(startTime)
        , m_endTime(endTime)
    { }

    void setTrajectories(const std::map<uint32_t, Trajectory>&);
    void mergeTrajectories(const std::map<uint32_t, Trajectory>&);
    const std::map<uint32_t, Trajectory>& trajectories() const { return m_trajectories; }
    uint32_t startTime() const { return m_startTime; }
    void setStartTime(uint32_t startTime) { m_startTime = startTime; }
    uint32_t endTime() const { return m_endTime; }
    void setEndTime(uint32_t endTime) { m_endTime = endTime; }
    void clearFirstPoints();
    void dump() const;
    void dumpTrajectories() const;

private:
    uint32_t m_startTime;
    uint32_t m_endTime;
    // Stores the trajectories that belong to this flock, ordered by the id of the trajectory
    std::map<uint32_t, Trajectory> m_trajectories;
};

#endif  // FLOCK_H
