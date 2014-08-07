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
    static double flatDistanceSpherical(const GPSPoint&, const GPSPoint&);
    static double flatDistanceEllipsoidal(const GPSPoint&, const GPSPoint&);
    static double tunnelDistance(const GPSPoint&, const GPSPoint&);

private:
    static double m_earthRadius;
};

#endif // Utils_h
