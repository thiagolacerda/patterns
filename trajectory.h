#ifndef Trajectory_h
#define Trajectory_h

#include <memory>
#include <vector>

class GPSPoint;

class Trajectory {
public:
    Trajectory() { }

    Trajectory(unsigned long id)
        : m_id(id)
    { }

    void addPoint(const std::shared_ptr<GPSPoint>&);

    unsigned long id() const { return m_id; }
    void dump();

private:
    unsigned long m_id;
    std::vector<std::shared_ptr<GPSPoint>> m_points;
};

struct TrajectoryComp {
    bool operator() (const std::shared_ptr<Trajectory>& t1, const std::shared_ptr<Trajectory>& t2) const
    {
        return t1->id() < t2->id();
    }
};

#endif // Trajectory_h
