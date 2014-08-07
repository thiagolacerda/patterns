#include "utils.h"

#include <math.h>
#include "gpspoint.h"

double Utils::m_earthRadius = 6371.009;

double Utils::degreesToRadians(double degrees)
{
    return (degrees * M_PI) / 180.0;
}

double Utils::flatDistanceSpherical(const GPSPoint& point1, const GPSPoint& point2)
{
    double p1LatRad = Utils::degreesToRadians(point1.latitude());
    double p1LongRad = Utils::degreesToRadians(point1.longitude());
    double p2LatRad = Utils::degreesToRadians(point2.latitude());
    double p2LongRad = Utils::degreesToRadians(point2.longitude());

    return Utils::m_earthRadius * sqrt(pow(p2LatRad - p1LatRad, 2.0) +
        pow(cos((p2LatRad + p1LatRad) / 2.0) * (p2LongRad - p1LongRad), 2.0));
}

double Utils::flatDistanceEllipsoidal(const GPSPoint& point1, const GPSPoint& point2)
{
    double p1LatRad = Utils::degreesToRadians(point1.latitude());
    double p1LongRad = Utils::degreesToRadians(point1.longitude());
    double p2LatRad = Utils::degreesToRadians(point2.latitude());
    double p2LongRad = Utils::degreesToRadians(point2.longitude());
    double meanLat = (p2LatRad + p1LatRad) / 2.0;

    // K1 = kilometers per degree of latitude difference
    double k1 = 111.13209 - 0.56605 * cos(2 * meanLat) + 0.0012 * cos(4 * meanLat);
    // K2 = kilometers per degree of longitude difference
    double k2 = 111.41513 * cos(meanLat) - 0.09455 * cos(3 * meanLat) + 0.00012 * cos(5 * meanLat);

    return sqrt(pow(k1 * (p2LatRad - p1LatRad), 2.0) + pow(k2 * (p2LongRad - p1LongRad), 2.0));
}

double Utils::tunnelDistance(const GPSPoint& point1, const GPSPoint& point2)
{
    double p1LatRad = Utils::degreesToRadians(point1.latitude());
    double p1LongRad = Utils::degreesToRadians(point1.longitude());
    double p2LatRad = Utils::degreesToRadians(point2.latitude());
    double p2LongRad = Utils::degreesToRadians(point2.longitude());

    double deltaX = cos(p2LatRad) * cos(p2LongRad) - cos(p1LatRad) * cos(p1LongRad);
    double deltaY = cos(p2LatRad) * sin(p2LongRad) - cos(p1LatRad) * sin(p1LongRad);
    double deltaZ = sin(p2LatRad) - sin(p1LatRad);
    return Utils::m_earthRadius * sqrt(pow(deltaX, 2.0) + pow(deltaY, 2.0) + pow(deltaZ, 2.0));
}
