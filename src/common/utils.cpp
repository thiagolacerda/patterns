#include "utils.h"

#include <math.h>
#include "config.h"
#include "gpspoint.h"

double Utils::m_earthRadius = 6371.009 * 1000;  // Earth radius in meters
double Utils::m_epsilon = 0.001;
double Utils::m_latDegMeterLen = 110567.238;
double Utils::m_longDegMeterLen = 111320.7;

bool Utils::fuzzyEqual(double a, double b)
{
    return fabs(a - b) < Utils::m_epsilon;
}

bool Utils::fuzzyLessEqual(double a, double b)
{
    return a < b || Utils::fuzzyEqual(a, b);
}

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
    /*
       This code is based in the routines found in http://www.whoi.edu/marine/ndsf/utility/NDSFutility.html

       // METERS_DEGLAT and METERS_DEGLON is the length of a degree of latitude and longitude at the 0,0 coord
       xx = (porg.slon - porg.olon)*METERS_DEGLON(porg.olat);
       yy = (porg.slat - porg.olat)*METERS_DEGLAT(porg.olat);

       r = sqrt(xx*xx + yy*yy);

       if(r) {
           ct = xx/r;
           st = yy/r;
           // angle is 0, cos(0) = 1 and sin(0) = 0
           xx = r * ( (ct * cos(angle)) + (st * sin(angle)) );
           yy = r * ( (st * cos(angle)) - (ct * sin(angle)) );
       }
       pxpos_mtrs = xx + porg.xoffset_mtrs;
       pypos_mtrs = yy + porg.yoffset_mtrs;

       var sxy={};
       sxy={x:pxpos_mtrs, y:pypos_mtrs};
     */

    double x = longitude * m_longDegMeterLen;
    double y = latitude * m_latDegMeterLen;
    double r = sqrt(x * x + y * y);
    if (r > 0) {
        x = r * (x / r);
        y = r * (y / r);
    }
    *latMeters = y;
    *longMeters = x;
}

void Utils::metersToLatLong(double latMeters, double longMeters, double *latitude, double *longitude)
{
    /*
       This code is based in the routines found in http://www.whoi.edu/marine/ndsf/utility/NDSFutility.html

       pxpos_mtrs = porg.x;
       pypos_mtrs = porg.y;
       xx = pxpos_mtrs - porg.xoffset_mtrs;
       yy = pypos_mtrs - porg.yoffset_mtrs;
       r = sqrt(xx*xx + yy*yy);

       if(r) {
           ct = xx/r;
           st = yy/r;
           xx = r * ( (ct * cos(angle))+ (st * sin(angle)) );
           yy = r * ( (st * cos(angle))- (ct * sin(angle)) );
       }

       // METERS_DEGLAT and METERS_DEGLON is the length of a degree of latitude and longitude at the 0,0 coord
       var plon = porg.olon + xx/METERS_DEGLON(olat);
       var plat = porg.olat + yy/METERS_DEGLAT(olat);

       var sll={};
       sll={slat:plat, slon:plon};
     */

    double x = longMeters;
    double y = latMeters;
    double r = sqrt(x * x + y * y);
    if (r > 0) {
        x = r * (x / r);
        y = r * (y / r);
    }
    *latitude = y / m_latDegMeterLen;
    *longitude = x / m_longDegMeterLen;
}

double Utils::distance(double lat1, double long1, double lat2, double long2, Utils::DistanceType type)
{
    if (Config::coordinateSystem() == Config::Cartesian || Config::coordinateSystem() == Config::CartesianNoConvert)
        return Utils::euclidianDistance(lat1, long1, lat2, long2);

    double (*distanceFunction)(double, double, double, double);
    switch (type) {
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
    double deltaX = p1X - p2X;
    double deltaY = p1Y - p2Y;
    return sqrt((deltaX * deltaX) + (deltaY * deltaY));
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
