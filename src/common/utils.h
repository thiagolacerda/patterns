#pragma once

#include <functional>
#include <memory>

class GPSPoint;

template<class It1, class It2>
using IntersectionCallback = std::function<void(const typename It1::value_type&, const typename It2::value_type&)>;

class Utils {
public:
    static double degreesToRadians(double degres);
    static double radiansToDegrees(double radians);
    static double distance(double p1X, double p1Y, double p2X, double p2Y);
    static double distance(const GPSPoint& point1, const GPSPoint& point2);
    static void latLongToMeters(double latitude, double longitude, double& latMeters, double& longMeters);
    static void metersToLatLong(double latMeters, double longMeters, double& latitude, double& longitude);
    static void midPoint(double x1, double y1, double x2, double y2, double& midX, double& midY);
    static void toVector(double x1, double y1, double x2, double y2, double& vectorX, double& vectorY);
    static double vectorLength(double vectorX, double vectorY);
    static void normalizedVector(double vectorX, double vectorY, double& normX, double& normY);
    static bool fuzzyEqual(double a, double b, double eps = Utils::m_epsilon);
    static bool fuzzyLessEqual(double, double);
    static bool fuzzyGreatEqual(double, double);
    static std::shared_ptr<GPSPoint> interpolate(const GPSPoint& p1, const GPSPoint& p2);
    template<typename T>
    static std::string toString(T value, unsigned precision);
    static std::string trim(const std::string& str);

    /*
    * Computes the intersection between two oredered id sets and returns the value.
    * A callback function can be specified to he called with the value type
    */
    template<class It1, class It2>
    static uint32_t intersection(It1 begin1, It1 end1, It2 begin2, It2 end2,
        const IntersectionCallback<It1, It2>& func = IntersectionCallback<It1, It2>())
    {
        auto lastElement1 = std::prev(end1, 1);
        auto lastElement2 = std::prev(end2, 1);
        if ((begin1->first < begin2->first && lastElement1->first < begin2->first) ||
            (begin2->first < begin1->first && lastElement2->first < begin1->first))
            return 0;

        uint32_t count = 0;
        while (begin1 != end1 && begin2 != end2) {
            if (begin1->first < begin2->first) {
                ++begin1;
            } else if (begin2->first < begin1->first) {
                ++begin2;
            } else {
                if (func)
                    func(*begin1, *begin2);
                ++count;
                ++begin1;
                ++begin2;
            }
        }
        return count;
    }

private:
    static constexpr double m_epsilon = 0.001;

    // Below is the length of a degree of latitude and longitude in 0,0 coordinate
    static constexpr double m_latDegMeterLen = 110567.238;
    static constexpr double m_longDegMeterLen = 111320.7;
};

