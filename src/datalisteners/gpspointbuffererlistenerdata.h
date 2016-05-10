#pragma once

#include <memory>
#include <unordered_map>
#include "processordata.h"

class GPSPoint;

class GPSPointBuffererListenerData : public ProcessorData {
public:
    GPSPointBuffererListenerData(const std::unordered_map<uint32_t, std::vector<std::shared_ptr<GPSPoint>>>& points,
            std::unordered_map<uint32_t, uint64_t>& presenceMap, uint32_t size, uint64_t timestamp)
        : m_points(points)
        , m_presenceMap(presenceMap)
        , m_extent(size)
        , m_timestamp(timestamp)
    {
    }

    const std::unordered_map<uint32_t, std::vector<std::shared_ptr<GPSPoint>>>& m_points;
    std::unordered_map<uint32_t, uint64_t>& m_presenceMap;
    uint32_t m_extent;
    uint64_t m_timestamp;
};

