#include "grid.h"

void Grid::addPoint(GPSPoint* point)
{
    m_points.push_back(point);
}

std::vector<GPSPoint*> Grid::closePointsTo(const GPSPoint& refPoint, double minDistance,
    Utils::DistanceType type) const
{
    double (*distanceFunction)(const GPSPoint&, const GPSPoint&);
    switch(type) {
    case Utils::FlatEllipsoidal:
        distanceFunction = Utils::flatDistanceEllipsoidal;
        break;
    case Utils::TunnelDistance:
        distanceFunction = Utils::tunnelDistance;
        break;
    default:
        distanceFunction = Utils::flatDistanceSpherical;
        break;
    }

    std::vector<GPSPoint*> closePoints;
    for (auto gpsPoint : m_points) {
        if (distanceFunction(refPoint, *gpsPoint) <= minDistance)
            closePoints.push_back(gpsPoint);
    }

    return closePoints;
}
