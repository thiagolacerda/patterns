#ifndef Grid_h
#define Grid_h

#include <vector>
#include "utils.h"

class GPSPoint;

class Grid {
public:
    Grid() {}
    Grid(const std::vector<GPSPoint*>& points)
        : m_points(points)
    { }

    void addPoint(GPSPoint*);
    std::vector<GPSPoint*> closePointsTo(const GPSPoint&, double minDistance,
        Utils::DistanceType type = Utils::FlatSpherical) const;

    void dump();

private:
    std::vector<GPSPoint*> m_points;
};

#endif // Grid_h
