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

#endif // Trajectory_h
