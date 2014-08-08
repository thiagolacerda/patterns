#include "trajectory.h"

#include <algorithm>
#include "gpspoint.h"

void Trajectory::addPoint(GPSPoint* point)
{
    m_points.push_back(point);
}

void Trajectory::mergePoints(const Trajectory& trajectory)
{
    std::vector<GPSPoint*> points = trajectory.m_points;
    for (auto point : points) {
        std::vector<GPSPoint*>::iterator iter = std::lower_bound(m_points.begin(), m_points.end(), point);
        if (iter == m_points.end() || (*iter)->timestamp() == point->timestamp())
            continue;

        m_points.insert(iter, point);
    }
}
