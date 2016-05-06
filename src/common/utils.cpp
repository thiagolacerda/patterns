#include "utils.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include "config.h"
#include "gpspoint.h"

bool Utils::fuzzyEqual(double a, double b, double eps)
{
    return fabs(a - b) < eps;
}

bool Utils::fuzzyLessEqual(double a, double b)
{
    return a < b || Utils::fuzzyEqual(a, b);
}

double Utils::degreesToRadians(double degrees)
{
    return (degrees * M_PI) / 180.0;
}

double Utils::radiansToDegrees(double radians)
{
    return (radians * 180.0) / M_PI;
}

void Utils::midPoint(double x1, double y1, double x2, double y2, double& midX, double& midY)
{
    midX = (x1 + x2) / 2.0;
    midY = (y1 + y2) / 2.0;
}

void Utils::toVector(double x1, double y1, double x2, double y2, double& vectorX, double& vectorY)
{
    vectorX = x2 - x1;
    vectorY = y2 - y1;
}

double Utils::vectorLength(double vectorX, double vectorY)
{
    return sqrt(pow(vectorX, 2.0) + pow(vectorY, 2.0));
}

void Utils::normalizedVector(double vectorX, double vectorY, double& normX, double& normY)
{
    double length = Utils::vectorLength(vectorX, vectorY);
    normX = vectorX / length;
    normY = vectorY / length;
}

void Utils::latLongToMeters(double latitude, double longitude, double& latMeters, double& longMeters)
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
    latMeters = y;
    longMeters = x;
}

void Utils::metersToLatLong(double latMeters, double longMeters, double& latitude, double& longitude)
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
    latitude = y / m_latDegMeterLen;
    longitude = x / m_longDegMeterLen;
}

double Utils::distance(double p1X, double p1Y, double p2X, double p2Y)
{
    double deltaX = p1X - p2X;
    double deltaY = p1Y - p2Y;
    return sqrt((deltaX * deltaX) + (deltaY * deltaY));
}

double Utils::distance(const GPSPoint& point1, const GPSPoint& point2)
{
    return Utils::distance(point1.longitudeMeters(), point1.latitudeMeters(),
        point2.longitudeMeters(), point2.latitudeMeters());
}

std::shared_ptr<GPSPoint> Utils::interpolate(const GPSPoint& p1, const GPSPoint& p2)
{
    double dLon = Utils::degreesToRadians(p2.longitude() - p1.longitude());

    //convert to radians
    double lat1Radians = Utils::degreesToRadians(p1.latitude());
    double long1Radians = Utils::degreesToRadians(p1.longitude());
    double lat2Radians = Utils::degreesToRadians(p2.latitude());

    double bx = cos(lat2Radians) * cos(dLon);
    double by = cos(lat2Radians) * sin(dLon);
    double finalLat = atan2(sin(lat1Radians) + sin(lat2Radians),
        sqrt((cos(lat1Radians) + bx) * (cos(lat1Radians) + bx) + by * by));
    double finalLong = long1Radians + atan2(by, cos(lat1Radians) + bx);

    return std::shared_ptr<GPSPoint>(new GPSPoint(Utils::radiansToDegrees(finalLat),
        Utils::radiansToDegrees(finalLong), (p1.timestamp() + p2.timestamp()) / 2, p1.trajectoryId()));
}

template<typename T>
std::string Utils::toString(T value, unsigned precision)
{
    std::ostringstream out;
    out << std::setprecision(precision) << value;
    return out.str();
}
template std::string Utils::toString<double>(double T, unsigned precision);
template std::string Utils::toString<float>(float T, unsigned precision);
