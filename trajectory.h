#ifndef Trajectory_h
#define Trajectory_h

#include <vector>

class GPSPoint;

class Trajectory {
public:
    Trajectory() { }

    Trajectory(long id)
        : m_id(id)
    { }

    Trajectory(long id, const std::vector<GPSPoint*>& points)
        : m_id(id)
        , m_points(points)
    { }

    void addPoint(GPSPoint*);
    void mergePoints(const Trajectory&);

    long id() const { return m_id; }

private:
    int m_id;
    std::vector<GPSPoint*> m_points;
};

#endif // Trajectory_h
