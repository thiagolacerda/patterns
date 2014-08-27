#include "utils.h"

#include <math.h>
#include "config.h"
#include "gpspoint.h"

double Utils::m_earthRadius = 6371.009;
double Utils::m1 = 111132.92;
double Utils::m2 = -559.82;
double Utils::m3 = 1.175;
double Utils::m4 = -0.0023;
double Utils::p1 = 111412.84;
double Utils::p2 = -93.5;
double Utils::p3 = 0.118;

double Utils::degreesToRadians(double degrees)
{
    return (degrees * M_PI) / 180.0;
}

void Utils::midPoint(double x1, double y1, double x2, double y2, double* midX, double* midY)
{
    *midX = (x1 + x2) / 2.0;
    *midY = (y1 + y2) / 2.0;
}

void Utils::toVector(double x1, double y1, double x2, double y2, double* vectorX, double* vectorY)
{
    *vectorX = x2 - x1;
    *vectorY = y2 - y1;
}

double Utils::vectorLength(double vectorX, double vectorY)
{
    return sqrt(pow(vectorX, 2.0) + pow(vectorY, 2.0));
}

void Utils::normalizedVector(double vectorX, double vectorY, double* normX, double* normY)
{
    double length = Utils::vectorLength(vectorX, vectorY);
    *normX = vectorX / length;
    *normY = vectorY / length;
}

void Utils::latLongToMeters(double latitude, double longitude, double* latMeters, double* longMeters)
{
    // This is the algorithm used by the National Geospatial-Intelligence Agency
    // check: http://msi.nga.mil/NGAPortal/MSI.portal
    // and: http://msi.nga.mil/NGAPortal/MSI.portal?_nfpb=true&_st=&_pageLabel=msi_portal_page_145&calcCode=03

    double latRad = Utils::degreesToRadians(latitude);

    // Calculate the length of a degree of latitude and longitude in meters
    double latLen = Utils::m1 + (Utils::m2 * cos(2 * latRad)) + (Utils::m3 * cos(4 * latRad)) +
        (Utils::m4 * cos(6 * latRad));
    double longLen = (Utils::p1 * cos(latRad)) + (Utils::p2 * cos(3 * latRad)) + (Utils::p3 * cos(5 * latRad));
    *latMeters = latitude * latLen;
    *longMeters = longitude * longLen;

}

double Utils::distance(double lat1, double long1, double lat2, double long2, Utils::DistanceType type)
{
    if (Config::coordinateSystem() == Config::Cartesian)
        return Utils::euclidianDistance(lat1, long1, lat2, long2);

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

    return distanceFunction(Utils::degreesToRadians(lat1), Utils::degreesToRadians(long1),
        Utils::degreesToRadians(lat2), Utils::degreesToRadians(long2));
}

double Utils::distance(const GPSPoint& point1, const GPSPoint& point2, Utils::DistanceType type)
{
    return Utils::distance(point1.latitude(), point1.longitude(), point2.latitude(), point2.longitude(), type);
}

double Utils::euclidianDistance(double p1X, double p1Y, double p2X, double p2Y)
{
    return sqrt(pow(p1X - p2X, 2.0) + pow(p1Y - p2Y, 2.0));
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
