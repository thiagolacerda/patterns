#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

class Disk;
class GPSPoint;

using OnDiskReadyFunc = std::function<void(const std::shared_ptr<Disk>&)>;

class FlockUtils {
public:
    FlockUtils()
        : m_radius(0)
        , m_gridSize(0)
        , m_currentTimeSlot(0)
        , m_pastBufferSize(0)
        , m_futureBufferSize(0)
        , m_mask(0)
        , m_currentMask(0)
        , m_flockLength(0)
        , m_trajectoriesPerFlock(0)
    {
    }

    void setPastBufferSize(uint32_t pastBufferSize);
    void setFutureBufferSize(uint32_t futureBufferSize);
    void setFlockLength(uint32_t flockLength);
    void setCurrentTimeslot(uint64_t timeSlot);
    void setRadius(double radius);
    void setTrajectoriesPerFlock(uint32_t trajectoriesPerFlock) { m_trajectoriesPerFlock = trajectoriesPerFlock; }
    void cachePointPresence(uint32_t trajectoryId, uint64_t futurePresence);
    void clearPresenceMap() { m_flockPresenceMap.clear(); }
    void clearCache() { m_pointPresenceCache.clear(); }
    bool validPoint(uint32_t trajectoryId) { return m_pointPresenceCache.at(trajectoryId); }
    void updatePresenceMap(uint32_t trajectoryId);
    void shiftPresenceMap();
    void processExtendedGrid(const std::vector<std::shared_ptr<GPSPoint>>& grid, const OnDiskReadyFunc& func);

private:
    void computeDisks(GPSPoint* point1, GPSPoint* point2, std::shared_ptr<Disk>& disk1, std::shared_ptr<Disk>& disk2);
    void clusterPointsIntoDisks(const std::shared_ptr<Disk>& disk1, const std::shared_ptr<Disk>& disk2,
        const std::vector<std::shared_ptr<GPSPoint>>& points);
    double m_radius;
    double m_gridSize;
    uint64_t m_currentTimeSlot;
    uint32_t m_pastBufferSize;
    uint32_t m_futureBufferSize;
    uint64_t m_mask;
    uint64_t m_currentMask;
    uint32_t m_flockLength;
    uint32_t m_trajectoriesPerFlock;
    std::unordered_map<uint32_t, bool> m_pointPresenceCache;
    std::unordered_map<uint32_t, uint64_t> m_flockPresenceMap;
};

