#pragma once

#include <cstdint>
#include <map>
#include <memory>

class GPSPoint;

class Disk {
public:
    Disk();
#if defined(NEWDESIGN)
    Disk(double radius, double centerX, double centerY, uint64_t timestamp, uint32_t p1Id, uint32_t p2Id);
#else
    Disk(double centerX, double centerY, uint64_t timestamp, uint32_t p1Id, uint32_t p2Id);
#endif

    double centerX() const { return m_centerX; }
    double centerLongitude() const { return m_centerLongitude; }

    double centerY() const { return m_centerY; }
    double centerLatitude() const { return m_centerLatitude; }

    bool isPointInDisk(const std::shared_ptr<GPSPoint>& point);
    bool isPointInDisk(uint32_t id, double x, double y) const;

    uint64_t timestamp() const { return m_timestamp; }
    void addPoint(const std::shared_ptr<GPSPoint>& point);
    uint32_t numberOfPoints() const { return m_points.size(); }
    uint32_t countIntersection(Disk* other) const;
    const std::map<uint32_t, std::shared_ptr<GPSPoint>>& points() const { return m_points; }
#if !defined(NEWDESIGN)
    void dumpPoints() const;
    void dump() const;
#endif

private:
    double m_centerX;
    double m_centerLongitude;
    double m_centerY;
    double m_centerLatitude;
    double m_radius;
    uint64_t m_timestamp;
    uint32_t m_p1Id;
    uint32_t m_p2Id;
    std::map<uint32_t, std::shared_ptr<GPSPoint>> m_points;
};

