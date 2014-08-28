#include "trajectory.h"

#include <algorithm>
#include <iostream>
#include "gpspoint.h"

void Trajectory::addPoint(const std::shared_ptr<GPSPoint>& point)
{
    m_points.push_back(point);
}

void Trajectory::dump()
{
    std::cout << "Number of points: " << m_points.size() << std::endl;
    std::cout.precision(15);
    for (auto iter = m_points.begin(); iter != m_points.end(); ++iter) {
        std::cout << "\t";
        (*iter)->dump();
    }
}
