#ifndef Grid_h
#define Grid_h

#include <memory>
#include <vector>
#include "utils.h"

class GPSPoint;

class Grid {
public:
    Grid() {}

    void addPoint(const std::shared_ptr<GPSPoint>&);
    void dump();

private:
    std::vector<std::shared_ptr<GPSPoint>> m_points;
};

#endif // Grid_h
