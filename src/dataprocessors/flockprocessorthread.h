#pragma once

#include <condition_variable>
#include <cstdint>
#include <future>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <memory>
#include "disk.h"
#include "flockutils.h"
#include "diskmanager.h"
#include "sharedbuffer.h"

class Disk;
class GPSPoint;

using GridBuffer = SharedBuffer<std::vector<std::shared_ptr<GPSPoint>>>;
using DiskBuffer = SharedBuffer<std::shared_ptr<Disk>>;

class FlockProcessorThread {
public:
    FlockProcessorThread(GridBuffer& gridBuffer, const std::shared_ptr<FlockUtils>& flockUtils)
        : m_flockUtils(flockUtils)
        , m_gridBuffer(gridBuffer)
    {
    }

    void start();
    std::vector<std::shared_ptr<Disk>> get();

private:
    std::vector<std::shared_ptr<Disk>> processDisks();
    std::vector<std::shared_ptr<Disk>> processGrids();
    std::future<std::vector<std::shared_ptr<Disk>>> m_future;

    std::shared_ptr<FlockUtils> m_flockUtils;
    DiskManager m_diskManager;
    GridBuffer& m_gridBuffer;
    DiskBuffer m_disksBuffer;
};

