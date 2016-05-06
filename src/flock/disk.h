#ifndef DISK_H
#define DISK_H

#include <cstdint>
#include <map>
#include <memory>
#include <unordered_set>
#include <vector>
#include "trajectory.h"

class Grid;

class Disk {
public:
    Disk();
    Disk(double centerX, double centerY, uint64_t timestamp);

    double centerX() const { return m_centerX; }
    double centerLongitude() const { return m_centerLongitude; }

    double centerY() const { return m_centerY; }
    double centerLatitude() const { return m_centerLatitude; }

    bool isPointInDisk(const std::shared_ptr<GPSPoint>& point);

    uint64_t timestamp() const { return m_timestamp; }
    void addPoint(const std::shared_ptr<GPSPoint>&);
    uint32_t numberOfPoints() const { return m_points.size(); }
    uint32_t countIntersection(Disk* other) const;
    const std::map<uint32_t, std::shared_ptr<GPSPoint>>& points() const { return m_points; }
    void dumpPoints() const;
    void dump() const;

private:
    double m_centerX;
    double m_centerLongitude;
    double m_centerY;
    double m_centerLatitude;
    double m_radius;
    uint64_t m_timestamp;
    std::map<uint32_t, std::shared_ptr<GPSPoint>> m_points;
};

#endif  // DISK_H
