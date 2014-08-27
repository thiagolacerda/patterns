#ifndef Trajectory_h
#define Trajectory_h

#include <vector>

class GPSPoint;

class Trajectory {
public:
    Trajectory() { }

    Trajectory(unsigned long id)
        : m_id(id)
    { }

    Trajectory(unsigned long id, const std::vector<GPSPoint*>& points)
        : m_id(id)
        , m_points(points)
    { }

    void addPoint(GPSPoint*);
    void mergePoints(const Trajectory&);

    unsigned long id() const { return m_id; }
    void dump();

private:
    unsigned long m_id;
    std::vector<GPSPoint*> m_points;
};

#endif // Trajectory_h
