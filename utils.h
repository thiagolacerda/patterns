#ifndef Utils_h
#define Utils_h

class GPSPoint;

class Utils {
public:
    enum DistanceType {
        FlatSpherical,
        FlatEllipsoidal,
        TunnelDistance
    };

    static double degreesToRadians(double);
    static double distance(double lat1, double long1, double lat2, double long2,
        Utils::DistanceType type = FlatSpherical);
    static double distance(const GPSPoint&, const GPSPoint&, Utils::DistanceType type = FlatSpherical);
    static void latLongToMeters(double latitude, double longitude, double* latMeters, double* longMeters);

private:
    static double euclidianDistance(double p1X, double p1Y, double p2X, double p2Y);
    static double flatDistanceSpherical(double p1LatRad, double p1LongRad, double p2LatRad, double p2LongRad);
    static double flatDistanceEllipsoidal(double p1LatRad, double p1LongRad, double p2LatRad, double p2LongRad);
    static double tunnelDistance(double p1LatRad, double p1LongRad, double p2LatRad, double p2LongRad);
    static double m_earthRadius;

    // Some constants used in lat/long to meters conversion
    // Latitude calculation term 1
    static double m1;
    // Latitude calculation term 2
    static double m2;
    // Latitude calculation term 3
    static double m3;
    // Latitude calculation term 4
    static double m4;
    // Longitude calculation term 1
    static double p1;
    // Longitude calculation term 2
    static double p2;
    // Longitude calculation term 3
    static double p3;
};

#endif // Utils_h
