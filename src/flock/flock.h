#pragma once

#include <atomic>
#include <map>
#include "trajectory.h"

// A flock is composed of a set of trajectories (which are composed by a set of points), with a start and end time
class Flock {
public:
    Flock(uint64_t startTime, uint64_t endTime)
        : m_startTime(startTime)
        , m_endTime(endTime)
        , m_id(0)
    { }

    Flock()
        : Flock(0, 0)
    { }

    void setTrajectoriesFromPoints(const std::map<uint32_t, std::shared_ptr<GPSPoint>>& trajectories);
    void setTrajectories(const std::map<uint32_t, Trajectory>& trajectories);
    const std::map<uint32_t, Trajectory>& trajectories() const { return m_trajectories; }
    uint64_t startTime() const { return m_startTime; }
    void setStartTime(uint64_t startTime) { m_startTime = startTime; }
    uint64_t endTime() const { return m_endTime; }
    void setEndTime(uint64_t endTime) { m_endTime = endTime; }
    void clearFirstPoints();
    void assignId() { m_id = Flock::nextId(); }
    uint64_t id() const { return m_id; }
    static unsigned long long nextId();
#if !defined(NEWDESIGN)
    void dump() const;
    void dumpTrajectories() const;
#endif

private:
    uint64_t m_startTime;
    uint64_t m_endTime;
    // Stores the trajectories that belong to this flock, ordered by the id of the trajectory
    std::map<uint32_t, Trajectory> m_trajectories;
    static std::atomic_ullong m_globalIds;
    uint64_t m_id;
};

