#ifndef Flock_h
#define Flock_h

#include <map>
#include "trajectory.h"


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
    std::map<unsigned long, Trajectory> m_trajectories;
};

#endif // Flock_h
