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

void Trajectory::mergePoints(const Trajectory& other)
{
    m_points.insert(m_points.end(), other.m_points.begin(), other.m_points.end());
}

void Trajectory::clearFirstPoints()
{
    if (m_points.empty())
        return;

    uint64_t startTime = m_points.at(0)->timestamp();
    uint32_t count = 0;
    for (auto iter = m_points.begin(); iter != m_points.end();) {
        if ((*iter)->timestamp() != startTime)
            return;

        count++;
        iter = m_points.erase(iter);
    }
}
