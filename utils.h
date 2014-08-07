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
    static double distance(const GPSPoint&, const GPSPoint&, Utils::DistanceType type = FlatSpherical);

private:
    static double flatDistanceSpherical(double p1LatRad, double p1LongRad, double p2LatRad, double p2LongRad);
    static double flatDistanceEllipsoidal(double p1LatRad, double p1LongRad, double p2LatRad, double p2LongRad);
    static double tunnelDistance(double p1LatRad, double p1LongRad, double p2LatRad, double p2LongRad);
    static double m_earthRadius;
};

#endif // Utils_h
