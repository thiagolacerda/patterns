#ifndef GPSPOINT_H
#define GPSPOINT_H

#include <cstdint>
#include <iostream>
#include "utils.h"

class GPSPoint {
public:
    GPSPoint();

    GPSPoint(double latitude, double longitude, uint64_t timestamp, uint32_t trajectoryId);

    double latitude() const { return m_latitude; }
    double latitudeMeters() const { return m_latitudeMeters; }

    double longitude() const { return m_longitude; }
    double longitudeMeters() const { return m_longitudeMeters; }

    uint64_t timestamp() const { return m_timestamp; }

    uint32_t trajectoryId() const { return m_trajectoryId; }

    double distanceToPoint(const GPSPoint& other);

    bool operator<(const GPSPoint& other) const;
    bool operator==(const GPSPoint& other) const;

    void dump();

    friend std::ostream& operator<<(std::ostream& stream, const GPSPoint& point);

private:
    double m_latitude;
    double m_latitudeMeters;
    double m_longitude;
    double m_longitudeMeters;
    uint64_t m_timestamp;
    uint32_t m_trajectoryId;
};

#endif  // GPSPOINT_H
