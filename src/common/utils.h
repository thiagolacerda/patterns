#ifndef UTILS_H
#define UTILS_H

class GPSPoint;

class Utils {
public:
    static double degreesToRadians(double degres);
    static double distance(double p1X, double p1Y, double p2X, double p2Y);
    static double distance(const GPSPoint& point1, const GPSPoint& point2);
    static void latLongToMeters(double latitude, double longitude, double* latMeters, double* longMeters);
    static void metersToLatLong(double latMeters, double longMeters, double *latitude, double *longitude);
    static void midPoint(double x1, double y1, double x2, double y2, double* midX, double* midY);
    static void toVector(double x1, double y1, double x2, double y2, double* vectorX, double* vectorY);
    static double vectorLength(double vectorX, double vectorY);
    static void normalizedVector(double vectorX, double vectorY, double* normX, double* normY);
    static bool fuzzyEqual(double, double);
    static bool fuzzyLessEqual(double, double);
    static bool fuzzyGreatEqual(double, double);

private:
    static constexpr double m_epsilon = 0.001;

    // Below is the length of a degree of latitude and longitude in 0,0 coordinate
    static constexpr double m_latDegMeterLen = 110567.238;
    static constexpr double m_longDegMeterLen = 111320.7;
};

#endif  // UTILS_H
