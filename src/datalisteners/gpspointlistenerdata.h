#pragma once

#include <memory>
#include <unordered_map>
#include "processordata.h"

class GPSPoint;

class GPSPointListenerExtraData {
public:
    GPSPointListenerExtraData(std::unordered_map<uint32_t, uint64_t>& presenceMap, uint32_t size)
        : m_presenceMap(presenceMap)
        , m_extent(size)
    {
    }

    std::unordered_map<uint32_t, uint64_t>& m_presenceMap;
    uint32_t m_extent;
};

class GPSPointListenerData : public ProcessorData {
public:
    GPSPointListenerData(const std::unordered_map<uint32_t, std::vector<std::shared_ptr<GPSPoint>>>& points,
            uint64_t timestamp)
        : m_points(points)
        , m_timestamp(timestamp)
    {
    }

    const std::unordered_map<uint32_t, std::vector<std::shared_ptr<GPSPoint>>>& m_points;
    uint64_t m_timestamp;
    std::shared_ptr<GPSPointListenerExtraData> m_extra;
};

