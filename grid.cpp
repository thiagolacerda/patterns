#include "grid.h"

void Grid::addPoint(GPSPoint* point)
{
    m_points.push_back(point);
}

std::vector<GPSPoint*> Grid::closePointsTo(const GPSPoint& refPoint, double minDistance,
    Utils::DistanceType type) const
{
    std::vector<GPSPoint*> closePoints;
    for (auto gpsPoint : m_points) {
        if (Utils::distance(refPoint, *gpsPoint, type) <= minDistance)
            closePoints.push_back(gpsPoint);
    }

    return closePoints;
}
