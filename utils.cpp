#include "utils.h"

#include <math.h>
#include "gpspoint.h"

double Utils::m_earthRadius = 6371.009;

double Utils::degreesToRadians(double degrees)
{
    return (degrees * M_PI) / 180.0;
}

double Utils::distance(const GPSPoint& point1, const GPSPoint& point2, Utils::DistanceType type)
{
    double (*distanceFunction)(double, double, double, double);
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

    return distanceFunction(Utils::degreesToRadians(point1.latitude()),
        Utils::degreesToRadians(point1.longitude()),
        Utils::degreesToRadians(point2.latitude()),
        Utils::degreesToRadians(point2.longitude()));
}

double Utils::flatDistanceSpherical(double p1LatRad, double p1LongRad, double p2LatRad, double p2LongRad)
{
    return Utils::m_earthRadius * sqrt(pow(p2LatRad - p1LatRad, 2.0) +
        pow(cos((p2LatRad + p1LatRad) / 2.0) * (p2LongRad - p1LongRad), 2.0));
}

double Utils::flatDistanceEllipsoidal(double p1LatRad, double p1LongRad, double p2LatRad, double p2LongRad)
{
    double meanLat = (p2LatRad + p1LatRad) / 2.0;

    // K1 = kilometers per degree of latitude difference
    double k1 = 111.13209 - 0.56605 * cos(2 * meanLat) + 0.0012 * cos(4 * meanLat);
    // K2 = kilometers per degree of longitude difference
    double k2 = 111.41513 * cos(meanLat) - 0.09455 * cos(3 * meanLat) + 0.00012 * cos(5 * meanLat);

    return sqrt(pow(k1 * (p2LatRad - p1LatRad), 2.0) + pow(k2 * (p2LongRad - p1LongRad), 2.0));
}

double Utils::tunnelDistance(double p1LatRad, double p1LongRad, double p2LatRad, double p2LongRad)
{
    double deltaX = cos(p2LatRad) * cos(p2LongRad) - cos(p1LatRad) * cos(p1LongRad);
    double deltaY = cos(p2LatRad) * sin(p2LongRad) - cos(p1LatRad) * sin(p1LongRad);
    double deltaZ = sin(p2LatRad) - sin(p1LatRad);
    return Utils::m_earthRadius * sqrt(pow(deltaX, 2.0) + pow(deltaY, 2.0) + pow(deltaZ, 2.0));
}
