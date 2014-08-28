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

    void dump();

private:
    std::vector<GPSPoint*> m_points;
};

#endif // Grid_h
