#ifndef Flock_h
#define Flock_h

#include <map>
#include "trajectory.h"

// A flock is composed of a set of trajectories (which are composed by a set of points), with a start and end time
class Flock {
public:
    Flock()
        : m_startTime(0)
        , m_endTime(0)
    { }

    Flock(unsigned long startTime, unsigned long endTime)
        : m_startTime(startTime)
        , m_endTime(endTime)
    { }

    void setTrajectories(const std::map<unsigned long, Trajectory>&);
    void mergeTrajectories(const std::map<unsigned long, Trajectory>&);
    const std::map<unsigned long, Trajectory>& trajectories() const { return m_trajectories; }
    unsigned long startTime() const { return m_startTime; }
    void setStartTime(unsigned long startTime) { m_startTime = startTime; }
    unsigned long endTime() const { return m_endTime; }
    void setEndTime(unsigned long endTime) { m_endTime = endTime; }
    void clearFirstPoints();
    void dump() const;
    void dumpTrajectories() const;

private:
    unsigned long m_startTime;
    unsigned long m_endTime;
    // Stores the trajectories that belong to this flock, ordered by the id of the trajectory
    std::map<unsigned long, Trajectory> m_trajectories;
};

#endif // Flock_h
